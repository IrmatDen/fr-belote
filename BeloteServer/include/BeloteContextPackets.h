#ifndef BELOTESERVER_BELOTECONTEXTPACKETS_H
#define BELOTESERVER_BELOTECONTEXTPACKETS_H

#include <string>
#include <SFML/Network.hpp>

enum BeloteContextPacketType
{
	// Setup packets
	BCPT_AvailablePos,
	BCPT_PlayerChoosePos,
};

inline sf::Packet& operator<<(sf::Packet& packet, BeloteContextPacketType pt)		{ packet << (sf::Uint32)pt; return packet; }
inline sf::Packet& operator>>(sf::Packet& packet, BeloteContextPacketType &pt)		{ packet >> (sf::Uint32&)pt; return packet; }

#endif
