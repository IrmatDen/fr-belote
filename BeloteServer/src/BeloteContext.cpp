#include <iostream>
#include <algorithm>
#include <bitset>
#include <functional>
#include <iterator>

#include <SFML/Network.hpp>

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
