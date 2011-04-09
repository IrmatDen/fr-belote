#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <utility>

#include "Timer.h"
#include "Tools.h"

#include "IASocket.h"
#include "BeloteContext.h"

using namespace std;

namespace
{
	class BotNameProvider
	{
	public:
		typedef std::pair<int, std::string> NameInfo;

	public:
		static NameInfo GetName()
		{
			Init();

			const int nameIdx = FreeNames.front();
			FreeNames.pop();

			return std::make_pair(nameIdx, BotNames[nameIdx]);
		}

		static void FreeName(int nameIdx)
		{
			FreeNames.push(nameIdx);
		}

	private:
		static void Init()
		{
			if (BotNameProviderInitialized)
				return;

			for (int i = 0; i != BotNamesCount; i++)
				FreeNames.push(i);

			BotNameProviderInitialized = true;
		}

	private:
		static const int			BotNamesCount = 4;
		static const std::string	BotNames[BotNamesCount];
		static bool					BotNameProviderInitialized;
		static std::queue<int>		FreeNames;
	};
	
	const std::string	BotNameProvider::BotNames[BotNamesCount]	= { "Escartefigue", "Panisse", "César", "M. Brun" };
	bool				BotNameProvider::BotNameProviderInitialized = false;
	std::queue<int>		BotNameProvider::FreeNames;
}

IASocket::IASocket()
	: m_MySeat(-1)
{
	BotNameProvider::NameInfo ni = BotNameProvider::GetName();
	m_MyNameIndex	= ni.first;
	m_MyName		= ni.second;
	Connect("127.0.0.1", m_MyName);
}

IASocket::~IASocket()
{
	BotNameProvider::FreeName(m_MyNameIndex);
}

void IASocket::OnConnectionStatusChanged(ConnectionStatus newStatus)
{
	if (newStatus == CS_Connected)
	{
		SendChatMessage("Salut");
	}
}

void IASocket::OnPositionningReceived(const PositionningPacket &positionning)
{
	// Check that the seat hasn't been stolen; if it was, just take another one.
	if (m_MySeat >= 0 && positionning.m_Names[m_MySeat] == m_MyName)
		return;

	// Fill the first free spot
	boost::array<std::string, 4>::const_iterator it = std::find(positionning.m_Names.begin(),
		positionning.m_Names.end(),
		std::string());

	if (it == positionning.m_Names.end())
	{
		std::cout << "[IASocket::OnPositionningReceived] No more free seats!" << std::endl;
		Disconnect();
	}

	m_MySeat = std::distance(positionning.m_Names.begin(), it);
	ChoosePosition(BeloteContext::PlayerPositionStrings[m_MySeat]);

	m_PartnerSeat = m_MySeat + 2;
	if (m_PartnerSeat > 4)
		m_PartnerSeat = 4 - m_PartnerSeat;
}

void IASocket::OnCardsDealt(const CardsDealtPacket &cards)
{
	m_MyHand = cards.m_Cards;
}

void IASocket::OnPotentialAssetReceived(const std::string &assetCard)
{
	m_Asset = assetCard;
}

void IASocket::OnAskingRevealedAsset()
{
	// Count the number of cards I have at the proposed asset to deduce if it's worth trying.
	const size_t n = CountCardsForAsset(m_Asset.front());
	FinalizeAssetProposal(m_Asset.front(), n);
}

void IASocket::OnAskingAnotherAsset()
{
	// Check which colours I'd be able to play
	std::string potentialAssets(ColourPreffixes);
	const size_t assetPos = potentialAssets.find(m_Asset.front());
	potentialAssets.erase(assetPos, 1);

	// Count the number of cards I have at each potential asset to deduce if it's worth trying.
	size_t cardsCount[3];
	size_t idx = 0;
	size_t bestOption = 0;

	for_each(potentialAssets.begin(), potentialAssets.end(),
		[&] (const char &potentialAsset)
		{
			cardsCount[idx] = CountCardsForAsset(potentialAsset);
			if (cardsCount[idx] > cardsCount[bestOption])
				bestOption = idx;

			idx++;
		} );

	FinalizeAssetProposal(potentialAssets[bestOption], cardsCount[bestOption]);
}

size_t IASocket::CountCardsForAsset(char asset) const
{
	return accumulate(m_MyHand.begin(), m_MyHand.end(), 0,
		[&] (size_t v, const std::string &card) -> size_t
		{
			if (card.empty())
				return v;

			return v + (card.front() == asset ? 1 : 0);
		} );
}

bool IASocket::HasBelote(char asset) const
{
	if (m_MyHand.end() == find(m_MyHand.begin(), m_MyHand.end(), asset + string("Q")))
		return false;

	return m_MyHand.end() != find(m_MyHand.begin(), m_MyHand.end(), asset + string("K"));
}

void IASocket::FinalizeAssetProposal(char asset, size_t cardsCountInHand)
{
	// Check probablity to try to play it
	const float probToTake[6] = {0, 0, 0.55f, 0.7f, 0.875f, 1.f };
	float p = probToTake[cardsCountInHand];
	if (HasBelote(asset))
		p += 0.15f;

	const float v = sf::Randomizer::Random(0.f, 1.f);

	// And act
	if (v > p)
		DelayReaction(boost::bind(&ClientSocket::RefuseAsset, this));
	else
		DelayReaction(boost::bind(&ClientSocket::AcceptAsset, this, string() + asset));
}

void IASocket::OnAcceptedAsset(const AcceptedAssetPacket &acceptedAsset)
{
	m_Asset = acceptedAsset.m_Asset;

	const bool botIsInNSTeam = botInNSTeam();
	m_AssetTakenByOpponent =	( botIsInNSTeam && !acceptedAsset.m_AcceptedByNSTeam) ||
								(!botIsInNSTeam &&  acceptedAsset.m_AcceptedByNSTeam);
}

void IASocket::OnWaitingPlay(const WaitingPlayPacket &waitingPlay)
{
	int playedCard = sf::Randomizer::Random(0, waitingPlay.m_PossibleCardsCount - 1);
	DelayReaction(boost::bind(&ClientSocket::PlayCard, this, waitingPlay.m_PossibleCards[playedCard]));
}
