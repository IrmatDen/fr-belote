#ifndef BELOTESERVER_BELOTECONTEXTTYPES_H
#define BELOTESERVER_BELOTECONTEXTTYPES_H

#include <boost/array.hpp>

#include <SFML/Config.hpp>
#include <SFML/Network.hpp>

enum PlayerPosition
{
	PP_South,
	PP_West,
	PP_North,
	PP_East,

	_PP_Count
};

enum TeamIndex
{
	TI_None = -1,
	TI_NorthSouth,
	TI_WestEast,

	_TI_Count
};

typedef boost::array<std::string, 8>	PlayerHand;

enum PlayDirection
{
	PD_CW,
	PD_CCW,
};

struct BeloteContextRuleSet
{
	BeloteContextRuleSet() : m_PlayDir(PD_CW), m_WinningScore(1000), m_AllowBots(false)
	{ ; }

	PlayDirection	m_PlayDir;
	sf::Uint32		m_WinningScore;
	bool			m_AllowBots;
};

inline sf::Packet& operator<<(sf::Packet& packet, const BeloteContextRuleSet &bcrs)
{
	packet << (sf::Uint32)bcrs.m_PlayDir << bcrs.m_WinningScore;
	return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, BeloteContextRuleSet &bcrs)
{
	packet >> (sf::Uint32&)bcrs.m_PlayDir >> bcrs.m_WinningScore;
	return packet;
}

#endif
