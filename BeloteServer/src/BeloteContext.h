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

	static const size_t			ValidPlayerPositionCount = _PP_Count;
	static const std::string	PlayerPositionStrings[];

public:
	BeloteContext();

	void	Reset();

	// Players' management
	void	AddPlayer(ServerSocket *player);
	void	DropPlayer(ServerSocket *player);
	void	SetPlayerPos(ServerSocket *player, const std::string &posName);

private:
	void	SendCurrentPositioningToAll();
	void	SendCurrentPositioningTo(ServerSocket *player);

private:
	typedef std::vector<ServerSocket*>	Players;
	typedef Players::iterator			PlayersIt;
	typedef Players::const_iterator		PlayersConstIt;

	Players		m_UnplacedPlayers;
	Players		m_Players;
};

#endif
