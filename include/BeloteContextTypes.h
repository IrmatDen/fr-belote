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

enum PlayDirection
{
	PD_CW,
	PD_CCW,
};

typedef boost::array<std::string, 8>	PlayerHand;

//! Defines the 4 colour preffixes
extern const std::string ColourPreffixes;

//! Scoring functor; return the points value of the requested card according to the current asset
struct CardDefToScore : public std::binary_function<std::string, std::string, size_t>
{
	static const std::string	ValueOrder;
	static const std::string	ValueOrderAtAsset;
	static const int			NormalScores[];
	static const int			AssetScores[];

	size_t operator()(const std::string &currentAsset, const std::string &card) const
	{
		assert(card.size() > 0);

		if (card.front() == currentAsset.front())
			return AssetScores[ValueOrderAtAsset.rfind(card.c_str() + 1)];

		return NormalScores[ValueOrder.rfind(card.c_str() + 1)];
	}
};

//! Customizable rules sent by the host to the game's context.
struct BeloteContextRuleSet
{
	BeloteContextRuleSet() : m_PlayDir(PD_CW), m_WinningScore(1000), m_AllowBots(false)
	{ ; }

	PlayDirection	m_PlayDir;
	sf::Uint32		m_WinningScore;
	bool			m_AllowBots;
};

//! Allows streaming of ruleset to the network.
inline sf::Packet& operator<<(sf::Packet& packet, const BeloteContextRuleSet &bcrs)
{
	packet << (sf::Uint32)bcrs.m_PlayDir << bcrs.m_WinningScore;
	return packet;
}

//! Allows streaming of ruleset from the network.
inline sf::Packet& operator>>(sf::Packet& packet, BeloteContextRuleSet &bcrs)
{
	packet >> (sf::Uint32&)bcrs.m_PlayDir >> bcrs.m_WinningScore;
	return packet;
}

#endif
