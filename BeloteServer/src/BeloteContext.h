#ifndef BELOTESERVER_BELOTECONTEXT_H
#define BELOTESERVER_BELOTECONTEXT_H

#include <string>
#include <vector>

#include <SFML/Config.hpp>
#include <SFML/Network.hpp>

#include "ServerSocket.h"
#include "BeloteContextPackets.h"

class Server;

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

	enum TeamIndex
	{
		TI_NorthSouth,
		TI_WestEast,

		_TI_Count
	};

	static const size_t			ValidPlayerPositionCount = _PP_Count;
	static const std::string	PlayerPositionStrings[];

private:
	enum TurnEvent
	{
		TE_Dealing,
		TE_TakeAsset,
		TE_RefuseAsset,
		TE_TurnStarting,
	};

	static const std::string ValueOrder;
	static const std::string ValueOrderAtAsset;

public:
	BeloteContext(Server *server);
	~BeloteContext();

	void	Reset();

	// Network reactions
	void	HandleGameContextPacket(sf::Packet &packet, ServerSocket *sourcePlayer);

	// Players' management
	void	AddPlayer(ServerSocket *player);
	void	DropPlayer(ServerSocket *player);
	void	SetPlayerPos(ServerSocket *player, const std::string &posName);

	// Game management
	void	StartGame();
	void	AcceptAsset(const std::string &colourName);
	void	RefuseAsset();

private:
	void	SendCurrentPositioningToAll();
	void	SendCurrentPositioningTo(ServerSocket *player);

	void	NotifyStarting();
	void	NotifyTurnEvent(TurnEvent event, ServerSocket *player = 0);
	void	AskForAsset();

	void	InitDeck();
	void	ShuffleDeck();
	void	PreTurn();
	void	DealFirstPart();
	void	DealLastPart();

	void	StartTurn();
	void	AskToPlay();
	void	CardPlayed(const std::string &card);
	bool	PlayerHasColourInHand(const std::string &colour) const;
	bool	PlayerHasHigherCardThan() const;
	bool	PlayerMustCut() const;
	bool	PlayerCanOvercut() const;

	void	DumpAllCardsInHandTo(std::vector<std::string> &out) const;
	void	DumpColoursInHandTo(const std::string &colour, std::vector<std::string> &out) const;

	void	OrderHands();
	void	SendCurrentHands();

	// Tools
private:
	static PlayerPosition	GetNextPlayer(PlayerPosition pp)
	{
		if (PP_East == pp)
			return PP_South;
	
		return (PlayerPosition)(pp + 1);
	}

	struct IsSameColour : public std::binary_function<std::string, std::string, bool>
	{
		bool operator()(const std::string &testedColour, const std::string &card) const
		{
			return card.size() > 0 && card.front() == testedColour.front();
		}
	};

private:
	typedef std::vector<ServerSocket*>	Players;
	typedef Players::iterator			PlayersIt;
	typedef Players::const_iterator		PlayersConstIt;

	struct ContextData
	{
		Server		* m_Server;
		Players		m_UnplacedPlayers;
		Players		m_Players;

		sf::Uint32		m_Scores[_TI_Count];
		std::string		m_Deck[32];
		int				m_CurrentDeckPos;

		PlayerPosition	m_CurrentDealer;
		PlayerPosition	m_CurrentPlayer;

		std::string		m_PlayersHand[_PP_Count][8];
		int				m_RemainingCards[_PP_Count];

		std::string		m_PotentialAsset;
		bool			m_IsInFirstAnnouncePhase;
		std::string		m_CurrentAsset;
		TeamIndex		m_TeamAcceptingContract;

		int				m_CurrentlyPlayedCards;
		std::string		m_PlayedCards[_PP_Count];
	};

private:
	ContextData *d;
};

#endif
