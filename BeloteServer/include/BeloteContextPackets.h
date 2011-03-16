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
	BCPT_Dealing,
	BCPT_CardsDealt,
	BCPT_PotentialAsset,
	BCPT_AskRevealedAsset,
	BCPT_AskAnotherAsset,
	BCPT_AcceptAsset,
	BCPT_RefuseAsset,
	BCPT_AssetAccepted,
	BCPT_AssetRefused,
	BCPT_TurnStarting,
	BCPT_WaitingPlay,
	BCPT_PlayCard,
	BCPT_CardPlayed,
	BCPT_CurrentScores,
};

inline sf::Packet& operator<<(sf::Packet& packet, BeloteContextPacketType pt)		{ packet << (sf::Uint32)pt; return packet; }
inline sf::Packet& operator>>(sf::Packet& packet, BeloteContextPacketType &pt)		{ packet >> (sf::Uint32&)pt; return packet; }

#endif
