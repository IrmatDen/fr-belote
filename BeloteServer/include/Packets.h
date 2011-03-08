#ifndef BELOTESERVER_PACKETS_H
#define BELOTESERVER_PACKETS_H

#include <SFML/Config.hpp>

enum PacketType
{
	PT_ConnectionAccepted,
	PT_ClientName,
	PT_ClientLeave,
	PT_ServerShuttingDown,
	PT_ClientTextMessage,
};

inline sf::Packet& operator<<(sf::Packet& packet, PacketType pt)		{ packet << (sf::Uint32)pt; return packet; }
inline sf::Packet& operator>>(sf::Packet& packet, PacketType &pt)		{ packet >> (sf::Uint32&)pt; return packet; }

#endif
