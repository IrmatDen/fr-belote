#include <iostream>
#include <algorithm>
#include <bitset>
#include <functional>
#include <iterator>

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "Tools.h"

#include "BeloteContext.h"
#include "Packets.h"
#include "BeloteContextPackets.h"
#include "Server.h"

const std::string	BeloteContext::PlayerPositionStrings[] = { "South", "West", "North", "East", "Unknown" };

BeloteContext::BeloteContext(Server *server)
	: d(0)
{
	d = new ContextData();

	d->m_Server = server;

	d->m_UnplacedPlayers.reserve(ValidPlayerPositionCount);
	d->m_Players.resize(ValidPlayerPositionCount);

	Reset();
}

BeloteContext::~BeloteContext()
{
	delete d;
}

void BeloteContext::Reset()
{
	std::fill(d->m_UnplacedPlayers.begin(), d->m_UnplacedPlayers.end(), Players::value_type(0));
	std::fill(d->m_Players.begin(),d-> m_Players.end(), Players::value_type(0));
}

// Player management methods
void BeloteContext::AddPlayer(ServerSocket *player)
{
	d->m_UnplacedPlayers.push_back(player);
	SendCurrentPositioningTo(player);
}

void BeloteContext::DropPlayer(ServerSocket *player)
{
	PlayersIt playerIt = std::find(d->m_Players.begin(), d->m_Players.end(), player);
	if (playerIt != d->m_Players.end())
	{
		const size_t posIdx = std::distance(d->m_Players.begin(), playerIt);
		d->m_Players[posIdx]	= 0;
	
		// TODO Setup phase only? replace by IA in game?
		SendCurrentPositioningToAll();
	}
	else
	{
		PlayersIt playerIt = std::find(d->m_UnplacedPlayers.begin(), d->m_UnplacedPlayers.end(), player);
		d->m_UnplacedPlayers.erase(std::remove(d->m_UnplacedPlayers.begin(), d->m_UnplacedPlayers.end(), player), d->m_UnplacedPlayers.end());
	}
}

void BeloteContext::SetPlayerPos(ServerSocket *player, const std::string &posName)
{
	// Find position index.
	const std::string* posPtr	= std::find(PlayerPositionStrings, PlayerPositionStrings + sizeof(PlayerPositionStrings), posName);
	const size_t posIdx			= posPtr - PlayerPositionStrings;

	// Place or move player
	PlayersIt playerIt = std::find(d->m_UnplacedPlayers.begin(), d->m_UnplacedPlayers.end(), player);
	if (playerIt != d->m_UnplacedPlayers.end())
	{
		d->m_UnplacedPlayers.erase(std::remove(d->m_UnplacedPlayers.begin(), d->m_UnplacedPlayers.end(), player), d->m_UnplacedPlayers.end());

		// TODO conccurrency issue lurking here! last arrived impose his will!
		d->m_Players[posIdx] = player;
	}
	else
	{
		playerIt = std::find(d->m_Players.begin(), d->m_Players.end(), player);
		if (playerIt != d->m_Players.end())
		{
			(*playerIt)			= Players::value_type(0);
			d->m_Players[posIdx]	= player;
		}
		else
		{
			std::cout << "[Server] BeloteContext::SetPlayerPos: player not found!" << std::endl;
		}
	}

	SendCurrentPositioningToAll();
}

void BeloteContext::SendCurrentPositioningToAll()
{
	auto action = [&] (Players::const_reference cref) { if (cref) SendCurrentPositioningTo(cref); };

	std::for_each(d->m_UnplacedPlayers.begin(),	d->m_UnplacedPlayers.end(),	action);
	std::for_each(d->m_Players.begin(),			d->m_Players.end(),			action);
}

void BeloteContext::SendCurrentPositioningTo(ServerSocket *player)
{
	sf::Packet packet;
	packet << PT_GameContextPacket << BCPT_CurrentPositionning;
	std::for_each(d->m_Players.begin(), d->m_Players.end(),
			[&] (Players::const_reference cref)
			{
				if (cref)
					packet << cref->GetClientName();
				else
					packet << std::string();
			}
		);

	player->GetSocket().Send(packet);
}

// Game management methods
void BeloteContext::StartGame()
{
	NotifyStarting();

	// Init game state.
	InitDeck();
	ShuffleDeck();
	for(size_t i = 0; i < countof(d->m_Scores); i++)
		d->m_Scores[i] = 0;

	d->m_CurrentDealer = PP_South;
	PreTurn();
}

void BeloteContext::NotifyStarting()
{
	sf::Packet packet;
	packet << PT_GameContextPacket << BCPT_GameStarting;
	std::for_each(d->m_Players.begin(), d->m_Players.end(),
			[&] (Players::const_reference cref)
			{
				cref->GetSocket().Send(packet);
			}
	);
}

void BeloteContext::InitDeck()
{
	d->m_Deck[0] = "C1";
	d->m_Deck[1] = "C7";
	d->m_Deck[2] = "C8";
	d->m_Deck[3] = "C9";
	d->m_Deck[4] = "C10";
	d->m_Deck[5] = "CJ";
	d->m_Deck[6] = "CQ";
	d->m_Deck[7] = "CK";
	d->m_Deck[8] = "D1";
	d->m_Deck[9] = "D7";
	d->m_Deck[10] = "D8";
	d->m_Deck[11] = "D9";
	d->m_Deck[12] = "D10";
	d->m_Deck[13] = "DJ";
	d->m_Deck[14] = "DQ";
	d->m_Deck[15] = "DK";
	d->m_Deck[16] = "H1";
	d->m_Deck[17] = "H7";
	d->m_Deck[18] = "H8";
	d->m_Deck[19] = "H9";
	d->m_Deck[20] = "H10";
	d->m_Deck[21] = "HJ";
	d->m_Deck[22] = "HQ";
	d->m_Deck[23] = "HK";
	d->m_Deck[24] = "S1";
	d->m_Deck[25] = "S7";
	d->m_Deck[26] = "S8";
	d->m_Deck[27] = "S9";
	d->m_Deck[28] = "S10";
	d->m_Deck[29] = "SJ";
	d->m_Deck[30] = "SQ";
	d->m_Deck[31] = "SK";
}

void BeloteContext::ShuffleDeck()
{
	// more info available @ http://www.cigital.com/papers/download/developer_gambling.php
	// of course, the "safe seed" is not so safe here, at least at the moment.
	for(int ct = 0; ct != countof(d->m_Deck); ct++)
	{
		int x = sf::Randomizer::Random(ct, countof(d->m_Deck) - 1);
		std::swap(d->m_Deck[ct], d->m_Deck[x]);
	}
}

void BeloteContext::PreTurn()
{
	d->m_CurrentPlayer = GetNextPlayer(d->m_CurrentDealer);

	NotifyTurnEvent(TE_Dealing, d->m_Players[d->m_CurrentDealer]);
	DealFirstPart();
	
	d->m_IsInFirstAnnouncePhase = true;
	AskForAsset();
}

void BeloteContext::DealFirstPart()
{
	// TODO customize dealing with 3-2 / 2-3
	// Only supporting 3-2 for now
	int currentCardsCountToGive	= 3;
	PlayerPosition pp		= d->m_CurrentPlayer;
	d->m_CurrentDeckPos		= 0;
	int currentHandPos[4]	= { 0, 0, 0, 0 };
	for (int playerIdx = 0; playerIdx != _PP_Count * 2; playerIdx++)
	{
		for (int card = 0; card != currentCardsCountToGive; card++)
			d->m_PlayersHand[pp][currentHandPos[pp]++] = d->m_Deck[d->m_CurrentDeckPos++];

		pp = GetNextPlayer(pp);

		if (playerIdx == _PP_Count - 1)
			currentCardsCountToGive = 5 - currentCardsCountToGive;
	}

	// Ensure that the last 3 cards are not currently dealt.
	for (int c = 0; c != 3; c++)
	{
		for (int p = 0; p != _PP_Count; p++)
			d->m_PlayersHand[p][7 - c] = "";
	}

	OrderHands();
	SendCurrentHands();

	// Show potential asset.
	d->m_PotentialAsset = d->m_Deck[d->m_CurrentDeckPos++];
	std::for_each(d->m_Players.begin(), d->m_Players.end(),
		[&] (Players::reference player)
		{
			sf::Packet packet;
			packet << PT_GameContextPacket << BCPT_PotentialAsset;
			packet << d->m_PotentialAsset;

			player->GetSocket().Send(packet);
		}
	);
}

void BeloteContext::AskForAsset()
{
	sf::Packet packet;
	packet << PT_GameContextPacket;
	if (d->m_IsInFirstAnnouncePhase)
		packet << BCPT_AskRevealedAsset;
	else
		packet << BCPT_AskAnotherAsset;

	d->m_Players[d->m_CurrentPlayer]->GetSocket().Send(packet);
}

void BeloteContext::AcceptAsset(const std::string &colourName)
{
	d->m_CurrentAsset = colourName.front();
	d->m_TeamAcceptingContract = (d->m_CurrentPlayer == PP_South || d->m_CurrentPlayer == PP_North) ? TI_NorthSouth : TI_WestEast;

	NotifyTurnEvent(TE_TakeAsset, d->m_Players[d->m_CurrentPlayer]);

	DealLastPart();

	NotifyTurnEvent(TE_TurnStarting);
}

void BeloteContext::RefuseAsset()
{
	if (d->m_CurrentPlayer == d->m_CurrentDealer)
		d->m_IsInFirstAnnouncePhase = false;

	NotifyTurnEvent(TE_RefuseAsset, d->m_Players[d->m_CurrentPlayer]);

	d->m_CurrentPlayer = GetNextPlayer(d->m_CurrentPlayer);
	
	AskForAsset();
}

void BeloteContext::DealLastPart()
{
	PlayerPosition taker	= d->m_CurrentPlayer;
	d->m_CurrentPlayer		= GetNextPlayer(d->m_CurrentDealer);
	PlayerPosition pp		= d->m_CurrentPlayer;
	int currentHandPos[4]	= { 5, 5, 5, 5 };

	// Finish distributing the deck
	for (int i = 0; i != _PP_Count; i++)
	{
		int cardsToGive = (pp == taker) ? 2 : 3;
		for (int card = 0; card != cardsToGive; card++)
			d->m_PlayersHand[pp][currentHandPos[pp]++] = d->m_Deck[d->m_CurrentDeckPos++];

		pp = GetNextPlayer(pp);
	}

	// Give the shown card to the taker.
	d->m_PlayersHand[taker][7] = d->m_PotentialAsset;

	OrderHands();
	SendCurrentHands();

	// TODO Should start playing, isn't it?
}

void BeloteContext::OrderHands()
{
	auto compFunc = [](const std::string &c1, const std::string &c2) -> bool
					{
						if (c1 == "")	return false;
						if (c2 == "")	return true;
						
						static const std::string colourOrder("HSDC");
						const size_t	c1ColourIdx = colourOrder.find(c1.front()),
										c2ColourIdx = colourOrder.find(c2.front());
						if (c1ColourIdx < c2ColourIdx)	return true;
						if (c2ColourIdx < c1ColourIdx)	return false;
						
						static const std::string valueOrder("78910JQK1");
						const size_t	c1ValueIdx	= valueOrder.rfind(c1.c_str() + 1),
										c2ValueIdx	= valueOrder.rfind(c2.c_str() + 1);
						return c1ValueIdx < c2ValueIdx; // Same colour, sort by card.
					};

	for (int p = 0; p != _PP_Count; p++)
		std::sort(d->m_PlayersHand[p], d->m_PlayersHand[p] + countof(d->m_PlayersHand[p]), compFunc);
}

void BeloteContext::NotifyTurnEvent(TurnEvent event, ServerSocket *player /* = 0 */)
{
	sf::Packet packet;
	packet << PT_GameContextPacket;

	switch (event)
	{
	case TE_Dealing:
		packet << BCPT_Dealing << player->GetClientName().c_str();
		break;

	case TE_TakeAsset:
		packet << BCPT_AssetAccepted << player->GetClientName().c_str() << d->m_CurrentAsset;
		break;

	case TE_RefuseAsset:
		packet << BCPT_AssetRefused << player->GetClientName().c_str();
		break;

	case TE_TurnStarting:
		packet << BCPT_TurnStarting;
		break;
	}
	
	std::for_each(d->m_Players.begin(), d->m_Players.end(),
		[&] (Players::reference player) { player->GetSocket().Send(packet); }
	);
}

void BeloteContext::SendCurrentHands()
{
	int playerIndex = 0;
	std::for_each(d->m_Players.begin(), d->m_Players.end(),
		[&] (Players::reference player)
		{
			sf::Packet packet;
			packet << PT_GameContextPacket << BCPT_CardsDealt;
			for (int i = 0; i != 8; i++)
				packet << d->m_PlayersHand[playerIndex][i];

			player->GetSocket().Send(packet);

			playerIndex++;
		}
	);
}
