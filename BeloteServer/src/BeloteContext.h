#ifndef BELOTESERVER_BELOTECONTEXT_H
#define BELOTESERVER_BELOTECONTEXT_H

#include <string>
#include <vector>

#include <SFML/Config.hpp>

#include "ServerSocket.h"
#include "BeloteContextPackets.h"

class BeloteContext
{
public:
	enum PlayerPosition
	{
		PP_South,
		PP_West,
		PP_North,
		PP_East,

		_PP_Count
	};

	enum ScoreIndex
	{
		SI_NorthSouth,
		SI_WestEast,

		_SI_Count
	};

	static const size_t			ValidPlayerPositionCount = _PP_Count;
	static const std::string	PlayerPositionStrings[];

public:
	BeloteContext();
	~BeloteContext();

	void	Reset();

	// Players' management
	void	AddPlayer(ServerSocket *player);
	void	DropPlayer(ServerSocket *player);
	void	SetPlayerPos(ServerSocket *player, const std::string &posName);

	// Game management
	void	StartGame();
	void	DealFirstPart();

private:
	void	SendCurrentPositioningToAll();
	void	SendCurrentPositioningTo(ServerSocket *player);

	void	InitDeck();
	void	ShuffleDeck();

	PlayerPosition	GetNextPlayer(PlayerPosition pp) const;

private:
	typedef std::vector<ServerSocket*>	Players;
	typedef Players::iterator			PlayersIt;
	typedef Players::const_iterator		PlayersConstIt;

	struct ContextData
	{
		Players		m_UnplacedPlayers;
		Players		m_Players;

		sf::Uint32		m_Scores[_SI_Count];
		std::string		m_Deck[32];
		int				m_CurrentDeckPos;

		PlayerPosition	m_CurrentDealer;
		PlayerPosition	m_CurrentPlayer;
		std::string		m_PlayersHand[_PP_Count][8];
		std::string		m_PotentialAsset;
	};

private:
	ContextData *d;
};

#endif
