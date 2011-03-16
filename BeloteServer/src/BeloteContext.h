#ifndef BELOTESERVER_BELOTECONTEXT_H
#define BELOTESERVER_BELOTECONTEXT_H

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <SFML/Config.hpp>
#include <SFML/Network.hpp>

#include "ServerSocket.h"
#include "BeloteContextPackets.h"

class Server;
typedef boost::shared_ptr<Server> ServerPtr;

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
	BeloteContext(ServerPtr server);
	~BeloteContext();

	void	Reset();

	// Network reactions
	void	HandleGameContextPacket(sf::Packet &packet, ServerSocketPtr sourcePlayer);

	// Players' management
	void	AddPlayer(ServerSocketPtr player);
	void	DropPlayer(ServerSocketPtr player);
	void	SetPlayerPos(ServerSocketPtr player, const std::string &posName);

	// Game management
	void	StartGame();
	void	AcceptAsset(const std::string &colourName);
	void	RefuseAsset();

private:
	void	SendCurrentPositioningToAll();
	void	SendCurrentPositioningTo(ServerSocketPtr player);

	void	NotifyStarting();
	void	NotifyTurnEvent(TurnEvent event, ServerSocketPtr player = ServerSocketPtr());
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
	bool	PlayerMustCut() const;
	bool	PlayerCanOvercut() const;

	void	EvaluatePlayedCards(size_t *scores) const;
	size_t	GetMaxScoreFromPlayedCards() const;
	void	DumpAllCardsInHandTo(std::vector<std::string> &out) const;
	void	DumpColoursInHandTo(const std::string &colour, std::vector<std::string> &out) const;
	void	DumpOvercuttingCardsTo(std::vector<std::string> &out) const;

	void	OrderHands();
	void	SendCurrentHands();

private:
	typedef std::vector<ServerSocketPtr>	Players;
	typedef Players::iterator				PlayersIt;
	typedef Players::const_iterator			PlayersConstIt;

	struct ContextData
	{
		ServerPtr	m_Server;
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
	typedef boost::shared_ptr<ContextData>	ContextDataPtr;

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

	struct CardDefToScore : public std::binary_function<ContextDataPtr, std::string, size_t>
	{
		size_t operator()(const ContextDataPtr d, const std::string &card) const
		{
			if (card.size() == 0)
				return 0;

			if (card.front() == d->m_CurrentAsset.front())
				return 100 + ValueOrderAtAsset.rfind(card.c_str() + 1);

			if (card.front() == d->m_PlayedCards[0].front())
				return ValueOrder.rfind(card.c_str() + 1);

			return 0;
		}
	};

private:
	ContextDataPtr d;
};

typedef boost::shared_ptr<BeloteContext> BeloteContextPtr;

#endif
