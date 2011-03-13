#include <iostream>
#include <algorithm>
#include <bitset>
#include <iterator>

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "Tools.h"

#include "BeloteContext.h"
#include "Packets.h"
#include "BeloteContextPackets.h"

const std::string	BeloteContext::PlayerPositionStrings[] = { "South", "West", "North", "East", "Unknown" };

BeloteContext::BeloteContext()
{
	m_UnplacedPlayers.reserve(ValidPlayerPositionCount);
	m_Players.resize(ValidPlayerPositionCount);

	Reset();
}

void BeloteContext::Reset()
{
	std::fill(m_UnplacedPlayers.begin(), m_UnplacedPlayers.end(), Players::value_type(0));
	std::fill(m_Players.begin(), m_Players.end(), Players::value_type(0));
}

// Player management methods
void BeloteContext::AddPlayer(ServerSocket *player)
{
	m_UnplacedPlayers.push_back(player);
	SendCurrentPositioningTo(player);
}

void BeloteContext::DropPlayer(ServerSocket *player)
{
	PlayersIt playerIt = std::find(m_Players.begin(), m_Players.end(), player);
	if (playerIt != m_Players.end())
	{
		const size_t posIdx = std::distance(m_Players.begin(), playerIt);
		m_Players[posIdx]	= 0;
	
		// TODO Setup phase only? replace by IA in game?
		SendCurrentPositioningToAll();
	}
	else
	{
		PlayersIt playerIt = std::find(m_UnplacedPlayers.begin(), m_UnplacedPlayers.end(), player);
		m_UnplacedPlayers.erase(std::remove(m_UnplacedPlayers.begin(), m_UnplacedPlayers.end(), player), m_UnplacedPlayers.end());
	}
}

void BeloteContext::SetPlayerPos(ServerSocket *player, const std::string &posName)
{
	// Find position index.
	const std::string* posPtr	= std::find(PlayerPositionStrings, PlayerPositionStrings + sizeof(PlayerPositionStrings), posName);
	const size_t posIdx			= posPtr - PlayerPositionStrings;

	// Place or move player
	PlayersIt playerIt = std::find(m_UnplacedPlayers.begin(), m_UnplacedPlayers.end(), player);
	if (playerIt != m_UnplacedPlayers.end())
	{
		m_UnplacedPlayers.erase(std::remove(m_UnplacedPlayers.begin(), m_UnplacedPlayers.end(), player), m_UnplacedPlayers.end());

		// TODO conccurrency issue lurking here! last arrived impose his will!
		m_Players[posIdx] = player;
	}
	else
	{
		playerIt = std::find(m_Players.begin(), m_Players.end(), player);
		if (playerIt != m_Players.end())
		{
			(*playerIt)			= Players::value_type(0);
			m_Players[posIdx]	= player;
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

	std::for_each(m_UnplacedPlayers.begin(),	m_UnplacedPlayers.end(),	action);
	std::for_each(m_Players.begin(),			m_Players.end(),			action);
}

void BeloteContext::SendCurrentPositioningTo(ServerSocket *player)
{
	sf::Packet packet;
	packet << PT_GameContextPacket << BCPT_CurrentPositionning;
	std::for_each(m_Players.begin(), m_Players.end(),
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
	// Tell everyone we're about to start.
	sf::Packet packet;
	packet << PT_GameContextPacket << BCPT_GameStarting;
	std::for_each(m_Players.begin(), m_Players.end(),
			[&] (Players::const_reference cref)
			{
				cref->GetSocket().Send(packet);
			}
		);

	// Init game state.
	InitDeck();
	ShuffleDeck();
	for(size_t i = 0; i < countof(m_Scores); i++)
		m_Scores[i] = 0;
}

void BeloteContext::InitDeck()
{
	m_Deck[0] = "C1";
	m_Deck[1] = "C7";
	m_Deck[2] = "C8";
	m_Deck[3] = "C9";
	m_Deck[4] = "C10";
	m_Deck[5] = "CJ";
	m_Deck[6] = "CQ";
	m_Deck[7] = "CK";
	m_Deck[8] = "D1";
	m_Deck[9] = "D7";
	m_Deck[10] = "D8";
	m_Deck[11] = "D9";
	m_Deck[12] = "D10";
	m_Deck[13] = "DJ";
	m_Deck[14] = "DQ";
	m_Deck[15] = "DK";
	m_Deck[16] = "H1";
	m_Deck[17] = "H7";
	m_Deck[18] = "H8";
	m_Deck[19] = "H9";
	m_Deck[20] = "H10";
	m_Deck[21] = "HJ";
	m_Deck[22] = "HQ";
	m_Deck[23] = "HK";
	m_Deck[24] = "S1";
	m_Deck[25] = "S7";
	m_Deck[26] = "S8";
	m_Deck[27] = "S9";
	m_Deck[28] = "S10";
	m_Deck[29] = "SJ";
	m_Deck[30] = "SQ";
	m_Deck[31] = "SK";
}

void BeloteContext::ShuffleDeck()
{
	// more info available @ http://www.cigital.com/papers/download/developer_gambling.php
	for(int ct = 0; ct != countof(m_Deck); ct++)
	{
		int x = sf::Randomizer::Random(ct, countof(m_Deck) - 1);
		std::swap(m_Deck[ct], m_Deck[x]);
	}
}
