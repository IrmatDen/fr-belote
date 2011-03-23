#ifndef BELOTESERVER_BELOTECONTEXTPACKETS_H
#define BELOTESERVER_BELOTECONTEXTPACKETS_H

#include <string>

#include <boost/array.hpp>

#include <SFML/Config.hpp>
#include <SFML/Network.hpp>

#include "BeloteContextTypes.h"

enum BeloteContextPacketType
{
	// Setup packets
	BCPT_CurrentPositionning,
	BCPT_PlayerChoosePos,
	BCPT_PlayerUnseat,
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
	BCPT_NoAssetTaken,
	BCPT_TurnStarting,
	BCPT_WaitingPlay,
	BCPT_PlayCard,
	BCPT_CardPlayed,
	BCPT_CurrentScores,
	BCPT_TotalScores,
	BCPT_BeloteAnnounced,
	BCPT_RebeloteAnnounced,
	BCPT_ContractingTeamResult,
	BCPT_Litige,
	BCPT_MatchEnded,
};

inline sf::Packet& operator<<(sf::Packet& packet, BeloteContextPacketType pt)		{ packet << (sf::Uint32)pt; return packet; }
inline sf::Packet& operator>>(sf::Packet& packet, BeloteContextPacketType &pt)		{ packet >> (sf::Uint32&)pt; return packet; }

struct PositionningPacket
{
	boost::array<std::string, 4>	m_Names;
};

struct CardsDealtPacket
{
	PlayerHand	m_Cards;
};

struct AcceptedAssetPacket
{
	int				m_PlayerPos;
	std::string		m_Asset;
	bool			m_AcceptedByNSTeam;
};

struct WaitingPlayPacket
{
	sf::Uint32						m_PossibleCardsCount;
	boost::array<std::string, 8>	m_PossibleCards;
};

struct PlayedCardPacket
{
	std::string		m_Card;
	int				m_Player;
};

#endif
