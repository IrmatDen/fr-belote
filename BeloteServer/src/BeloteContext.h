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
		PP_Unknown,

		PP_Count
	};

	static const size_t			ValidPlayerPositionCount = PP_East;
	static const std::string	PlayerPositionStrings[];

public:
	BeloteContext();

	// Players' management
	void	AddPlayer(ServerSocket *player);
	void	DropPlayer(ServerSocket *player);
	void	SetPlayerPos(ServerSocket *player, const std::string &posName);

private:
	void	GetAvailablePositions(std::vector<PlayerPosition> &freePos);
	void	SendAvailablePositionsToAll();
	void	SendAvailablePositionsTo(ServerSocket *player, const std::vector<PlayerPosition> &freePos);

private:
	struct PlayerDesc
	{
		PlayerDesc() : m_Connection(0), m_Pos(PP_Unknown)	{ ; }
		bool	operator==(ServerSocket *socket) const		{ return socket == m_Connection; }

		ServerSocket	*	m_Connection;
		PlayerPosition		m_Pos;
	};

private:
	typedef std::vector<PlayerDesc>			PlayersDescs;
	typedef PlayersDescs::iterator			PlayersDescsIt;
	typedef PlayersDescs::const_iterator	PlayersDescsConstIt;

	PlayersDescs		m_Players;
};

#endif
