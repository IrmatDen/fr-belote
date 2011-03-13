#include <iostream>
#include <algorithm>
#include <bitset>
#include <iterator>

#include <SFML/Network.hpp>

#include "BeloteContext.h"

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
	
	// We must discard PP_Unknown from valid positions list.
	for(size_t i = 0; i != ValidPlayerPositionCount; i++)
	{
		if (!freePos[i])
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
	std::cout << "BeloteContext::SendAvailablePositionsTo(" << player->GetClientName() << ") :" << std::endl;
	std::ostream_iterator<PlayerPosition> outIt(std::cout, "\n");
	std::copy(freePos.begin(), freePos.end(), outIt);
}
