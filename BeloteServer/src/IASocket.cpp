#include <iostream>

#include "IASocket.h"
#include "BeloteContext.h"

const std::string IASocket::BotNames[] = { "Escartefigue", "Panisse", "César", "M. Brun" };
int IASocket::k_CurrentBotIdx = 0;

IASocket::IASocket()
	: m_Seated(false)
{
	Connect("127.0.0.1", BotNames[k_CurrentBotIdx]);
	k_CurrentBotIdx++;
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
	if (m_Seated)
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

	size_t freeSeatIdx = std::distance(positionning.m_Names.begin(), it);
	ChoosePosition(BeloteContext::PlayerPositionStrings[freeSeatIdx]);
	m_Seated = true;
}

void IASocket::OnCardsDealt(const CardsDealtPacket &cards)
{
	m_MyHand = cards.m_Cards;
}

void IASocket::OnPotentialAssetReceived(const std::string &assetCard)
{
	m_PotentialAsset = assetCard;
}

void IASocket::OnAskingRevealedAsset()
{
	float r = sf::Randomizer::Random(0.f, 1.f);
	if (r < 0.6f)
		RefuseAsset();
	else
		AcceptAsset(m_PotentialAsset);
}

void IASocket::OnAskingAnotherAsset()
{
	std::string potentialAssets("HSDC");
	potentialAssets.erase(potentialAssets.find_first_of(m_PotentialAsset.front()));

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
