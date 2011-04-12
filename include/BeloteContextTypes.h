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
typedef boost::array<size_t, 8>			Scores;

//! Class containing misc data used by belote
struct BeloteUtils
{
	//! Defines the 4 colour preffixes
	static const std::string	ColourPreffixes;
	static const std::string	ValueOrder;
	static const std::string	ValueOrderAtAsset;
	static const int			NormalScores[];
	static const int			AssetScores[];

	static int GetCardIndex(const std::string &card, const std::string &asset);
};


//! Scoring functor; return the points value of the requested card according to the current asset
struct CardDefToScore : public std::binary_function<std::string, std::string, size_t>
{
	size_t operator()(const std::string &currentAsset, const std::string &card) const
	{
		assert(card.size() > 0);

		if (card.front() == currentAsset.front())
			return BeloteUtils::AssetScores[BeloteUtils::ValueOrderAtAsset.rfind(card.c_str() + 1)];

		return BeloteUtils::NormalScores[BeloteUtils::ValueOrder.rfind(card.c_str() + 1)];
	}
};

//! Value functor: allows ordering of cards where asset cards are guaranteed to be the highest ones
struct CardDefToValue : public std::unary_function<std::string, size_t>
{
	CardDefToValue(const std::string &currentAsset, const std::string &requestedCard)
		: m_CurrentAsset(currentAsset), m_RequestedCard(requestedCard)
	{
	}

	CardDefToValue(const CardDefToValue &other)
		: m_CurrentAsset(other.m_CurrentAsset), m_RequestedCard(other.m_RequestedCard)
	{
	}

	const std::string &m_CurrentAsset;
	const std::string &m_RequestedCard;

	size_t operator()(const std::string &card) const
	{
		if (card.size() == 0)
			return 0;

		if (card.front() == m_CurrentAsset.front())
			return 100 + BeloteUtils::ValueOrderAtAsset.rfind(card.c_str() + 1);

		if (card.front() == m_RequestedCard.front())
			return BeloteUtils::ValueOrder.rfind(card.c_str() + 1);

		return 0;
	}

private:
	CardDefToValue& operator=(const CardDefToValue &other);
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
