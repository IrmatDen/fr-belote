#include <iostream>
#include <algorithm>
#include <bitset>
#include <iterator>

#include <SFML/Network.hpp>

#include "BeloteContext.h"
#include "Packets.h"
#include "BeloteContextPackets.h"

const std::string	BeloteContext::PlayerPositionStrings[] = { "South", "West", "North", "East", "Unknown" };

BeloteContext::BeloteContext()
{
	m_Players.reserve(ValidPlayerPositionCount);
}

void BeloteContext::AddPlayer(ServerSocket *player)
{
	if (m_Players.size() == ValidPlayerPositionCount)
	{
		std::cout << "[BeloteContext::AddPlayer] Trying to add too much player!" << std::endl;
		return;
	}

	PlayerDesc	addedPlayer;
	addedPlayer.m_Connection	= player;
	addedPlayer.m_Pos			= PP_Unknown;
	m_Players.push_back(addedPlayer);

	std::vector<PlayerPosition> freePos;
	GetAvailablePositions(freePos);
	SendAvailablePositionsTo(addedPlayer.m_Connection, freePos);
}

void BeloteContext::DropPlayer(ServerSocket *player)
{
	PlayersDescsIt descIt = std::find(m_Players.begin(), m_Players.end(), player);
	m_Players.erase(std::remove(m_Players.begin(), m_Players.end(), player), m_Players.end());
	
	// TODO Setup phase only?
	SendAvailablePositionsToAll();
}

void BeloteContext::GetAvailablePositions(std::vector<PlayerPosition> &outFreePos)
{
	outFreePos.reserve(ValidPlayerPositionCount);

	std::bitset<PP_Count> freePos(0);
	std::for_each(m_Players.begin(), m_Players.end(), [&] (const PlayerDesc &d) { freePos[d.m_Pos] = 1; } );
	
	for(size_t i = 0; i != PP_Count; i++)
	{
		if (!freePos[i] && i != PP_Unknown)
			outFreePos.push_back((PlayerPosition)i);
	}
}

void BeloteContext::SendAvailablePositionsToAll()
{
	std::vector<PlayerPosition> freePos;
	GetAvailablePositions(freePos);

	std::for_each(m_Players.begin(), m_Players.end(), [&] (const PlayerDesc &d) { SendAvailablePositionsTo(d.m_Connection, freePos); } );
}

void BeloteContext::SendAvailablePositionsTo(ServerSocket *player, const std::vector<PlayerPosition> &freePos)
{
	sf::Packet packet;
	packet << PT_GameContextPacket << BCPT_AvailablePos << freePos.size();
	std::for_each(freePos.begin(), freePos.end(), [&] (const PlayerPosition &pp) { packet << PlayerPositionStrings[pp]; } );

	player->GetSocket().Send(packet);
}
