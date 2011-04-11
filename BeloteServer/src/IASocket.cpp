#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <utility>

#include "Timer.h"
#include "Tools.h"

#include "IASocket.h"
#include "BeloteContext.h"

using namespace std;
using namespace boost;

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

	fill(m_PlayedCards.begin(), m_PlayedCards.end(), -1);
	m_AssetsPlayed = 0;
}

void IASocket::OnPotentialAssetReceived(const std::string &assetCard)
{
	m_Asset = assetCard;
}

void IASocket::OnAskingRevealedAsset()
{
	// Count the number of cards I have at the proposed asset to deduce if it's worth trying.
	const size_t n = CountCardsForColour(m_Asset.front());
	FinalizeAssetProposal(m_Asset.front(), n);
}

void IASocket::OnAskingAnotherAsset()
{
	// Check which colours I'd be able to play
	std::string potentialAssets(BeloteUtils::ColourPreffixes);
	const size_t assetPos = potentialAssets.find(m_Asset.front());
	potentialAssets.erase(assetPos, 1);

	// Count the number of cards I have at each potential asset to deduce if it's worth trying.
	size_t cardsCount[3];
	size_t idx = 0;
	size_t bestOption = 0;

	for_each(potentialAssets.begin(), potentialAssets.end(),
		[&] (const char &potentialAsset)
		{
			cardsCount[idx] = CountCardsForColour(potentialAsset);
			if (cardsCount[idx] > cardsCount[bestOption])
				bestOption = idx;

			idx++;
		} );

	FinalizeAssetProposal(potentialAssets[bestOption], cardsCount[bestOption]);
}

size_t IASocket::CountCardsForColour(char col) const
{
	return accumulate(m_MyHand.begin(), m_MyHand.end(), 0,
		[&] (size_t v, const std::string &card) -> size_t
		{
			if (card.empty())
				return v;

			return v + (card.front() == col ? 1 : 0);
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

	const bool botIsInNSTeam	= botInNSTeam();
	m_AssetTakenByOpponent		= botIsInNSTeam ^ acceptedAsset.m_AcceptedByNSTeam;
}

void IASocket::OnTurnStarting()
{
	fill(m_CurrentTurnCards.begin(), m_CurrentTurnCards.end(), "");
}

void IASocket::OnPlayedCard(const PlayedCardPacket &playedCard)
{
	// Update turn state
	m_CurrentTurnCards[playedCard.m_Player] = playedCard.m_Card;

	// Update play state
	const size_t colIdx = BeloteUtils::ColourPreffixes.find(playedCard.m_Card.front());
	assert(colIdx < 4);

	const int cardIdx = BeloteUtils::GetCardIndex(playedCard.m_Card, m_Asset);
	m_PlayedCards[colIdx * 8 + cardIdx] = playedCard.m_Player;

	if (playedCard.m_Card.front() == m_Asset.front())
		m_AssetsPlayed++;

	// And update my hand if it's me playing
	if (playedCard.m_Player == m_MySeat)
	{
		PlayerHand::iterator it = find(m_MyHand.begin(), m_MyHand.end(), playedCard.m_Card);
		assert(it != m_MyHand.end());

		swap(*it, string());
	}
}

void IASocket::OnWaitingPlay(const WaitingPlayPacket &waitingPlay)
{
	string cardToPlay;
	const bool isFirstPlaying = IsFirstPlayingInTurn();

	// First, check if we must try to have as much asset played (often because our team accepted the contract)
	if (isFirstPlaying && !m_AssetTakenByOpponent)
	{
		// Count how many assets I've in hand.
		const int assetsInHand = CountCardsForColour(m_Asset.front());
		if (assetsInHand > 0)
		{
			// Play either the highest or lowest depending on if I'm owning the turn
			const int assetsMissing = 8 - assetsInHand - m_AssetsPlayed;
			if (assetsMissing > 0)
			{
				// Find my highest card (ordering was performed server-side when cards were dealt)
				cardToPlay = GetStrongestCardForColour(m_Asset);
				assert(!cardToPlay.empty());

				if (!IsCardOwningTurn(cardToPlay))
				{
					cardToPlay = GetWeakestCardForColour(m_Asset);
					assert(!cardToPlay.empty());
				}
			}
		}
	}


	if (cardToPlay.empty())
	{
		int playedCard = sf::Randomizer::Random(0, waitingPlay.m_PossibleCardsCount - 1);
		cardToPlay = waitingPlay.m_PossibleCards[playedCard];
	}
	DelayReaction(boost::bind(&ClientSocket::PlayCard, this, cardToPlay));
}

bool IASocket::IsFirstPlayingInTurn() const
{
	array<std::string, 4>::const_iterator it =
		find_if_not(m_CurrentTurnCards.begin(), m_CurrentTurnCards.end(), mem_fun_ref(&string::empty));
	return it == m_CurrentTurnCards.end();
}

string IASocket::GetStrongestCardForColour(const string &colour) const
{
	PlayerHand::const_reverse_iterator it =
		find_if(m_MyHand.rbegin(), m_MyHand.rend(),
			[&] (const string &c) -> bool
			{
				if (c.empty()) return false;
				return c.front() == colour.front();
			} );

	if (it == m_MyHand.rend())
		return "";

	return *it;
}

string IASocket::GetWeakestCardForColour(const string &colour) const
{
	PlayerHand::const_iterator it =
		find_if(m_MyHand.begin(), m_MyHand.end(),
			[&] (const string &c) -> bool
			{
				if (c.empty()) return false;
				return c.front() == colour.front();
			} );

	if (it == m_MyHand.end())
		return "";

	return *it;
}

bool IASocket::IsCardOwningTurn(const std::string & card) const
{
	const int colourIndex			= BeloteUtils::ColourPreffixes.find(card.front());
	const size_t higherCardIndex	= colourIndex * 8 + BeloteUtils::GetCardIndex(card, m_Asset) + 1;
	if (higherCardIndex == m_PlayedCards.size())
		return true;

	DeckPlayed::const_iterator first	= m_PlayedCards.begin() + higherCardIndex;
	DeckPlayed::const_iterator last		= m_PlayedCards.begin() + higherCardIndex + (colourIndex + 1) * 8 - higherCardIndex;
	if (last > m_PlayedCards.end())
		last = m_PlayedCards.end();

	DeckPlayed::const_iterator it = find_if(first, last, [] (int c) -> bool { return c == -1; } );
	return it == last;
}
