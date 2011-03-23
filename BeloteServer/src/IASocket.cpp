#include <iostream>
#include <utility>
#include <queue>

#include "Tools.h"

#include "IASocket.h"
#include "BeloteContext.h"

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

	static const int			BotNamesCount = 4;
	static const std::string	BotNames[BotNamesCount];
	static bool					BotNameProviderInitialized;
	static std::queue<int>		FreeNames;
};
	
const std::string	BotNameProvider::BotNames[BotNamesCount]	= { "Escartefigue", "Panisse", "César", "M. Brun" };
bool				BotNameProvider::BotNameProviderInitialized = false;
std::queue<int>		BotNameProvider::FreeNames;

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
	float r = sf::Randomizer::Random(0.f, 1.f);
	if (r < 0.6f)
		RefuseAsset();
	else
		AcceptAsset(m_Asset);
}

void IASocket::OnAskingAnotherAsset()
{
	std::string potentialAssets("HSDC");
	const size_t assetPos = potentialAssets.find(m_Asset.front());
	potentialAssets.erase(assetPos, 1);

	float r = sf::Randomizer::Random(0.f, 1.f);
	if (r < 0.25f)
		RefuseAsset();
	else if (r < 0.5f)
		AcceptAsset(potentialAssets.substr(0, 1));
	else if (r < 0.75f)
		AcceptAsset(potentialAssets.substr(1, 1));
	else
		AcceptAsset(potentialAssets.substr(2, 1));
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
	PlayCard(waitingPlay.m_PossibleCards[playedCard]);
}
