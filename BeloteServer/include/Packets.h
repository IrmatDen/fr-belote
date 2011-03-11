#ifndef BELOTESERVER_PACKETS_H
#define BELOTESERVER_PACKETS_H

#include <SFML/Config.hpp>

enum PacketType
{
	PT_ConnectionAccepted,
	PT_ConnectionDeniedLobbyFull,
	PT_ClientName,
	PT_ClientLeave,
	PT_ServerShuttingDown,
	PT_ClientConnected,
	PT_ClientDisconnected,
	PT_ClientTextMessage,
	PT_ServerBroadcastTextMessage,
};

inline sf::Packet& operator<<(sf::Packet& packet, PacketType pt)		{ packet << (sf::Uint32)pt; return packet; }
inline sf::Packet& operator>>(sf::Packet& packet, PacketType &pt)		{ packet >> (sf::Uint32&)pt; return packet; }

#endif
