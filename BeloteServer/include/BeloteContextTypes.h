#ifndef BELOTESERVER_BELOTECONTEXTTYPES_H
#define BELOTESERVER_BELOTECONTEXTTYPES_H

#include <SFML/Config.hpp>
#include <SFML/Network.hpp>

enum PlayDirection
{
	PD_CW,
	PD_CCW,
};

struct BeloteContextRuleSet
{
	BeloteContextRuleSet() : m_PlayDir(PD_CW), m_WinningScore(1000)	{ ; }

	PlayDirection	m_PlayDir;
	sf::Uint32		m_WinningScore;
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
