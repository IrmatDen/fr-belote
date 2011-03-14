#ifndef BELOTESERVER_BELOTECONTEXTPACKETS_H
#define BELOTESERVER_BELOTECONTEXTPACKETS_H

#include <string>
#include <SFML/Network.hpp>

enum BeloteContextPacketType
{
	// Setup packets
	BCPT_CurrentPositionning,
	BCPT_PlayerChoosePos,
	BCPT_StartGameRequest,
	BCPT_GameStarting,
	BCPT_CardsDealt,
	BCPT_PotentialAsset,
};

inline sf::Packet& operator<<(sf::Packet& packet, BeloteContextPacketType pt)		{ packet << (sf::Uint32)pt; return packet; }
inline sf::Packet& operator>>(sf::Packet& packet, BeloteContextPacketType &pt)		{ packet >> (sf::Uint32&)pt; return packet; }

#endif
