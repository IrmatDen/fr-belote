#ifndef BELOTESERVER_BELOTECONTEXT_H
#define BELOTESERVER_BELOTECONTEXT_H

#include <string>
#include <vector>

#include <boost/array.hpp>

#include <SFML/Config.hpp>
#include <SFML/Network.hpp>

#include "ServerSocket.h"
#include "BeloteContextPackets.h"

class Server;
typedef std::shared_ptr<Server> ServerPtr;

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
		TI_None = -1,
		TI_NorthSouth,
		TI_WestEast,

		_TI_Count
	};

	typedef boost::array<std::string, 5> StringArray5;
	static const StringArray5	PlayerPositionStrings;

private:
	enum TurnEvent
	{
		TE_Dealing,
		TE_TakeAsset,
		TE_RefuseAsset,
		TE_NoAssetTaken,
		TE_TurnStarting,
		TE_BeloteAnnounced,
		TE_RebeloteAnnounced,
		TE_ScoresUpdated,
		TE_TotalScoresUpdated,
	};

	typedef boost::array<size_t, 8>	Scores;

	static const std::string	ValueOrder;
	static const std::string	ValueOrderAtAsset;

	static const int			NormalScores[];
	static const int			AssetScores[];
	static const sf::Uint32		LastTurnScore	= 10;
	static const sf::Uint32		BeloteScore		= 20;
	static const sf::Uint32		MinScoreToWin	= 82;
	static const sf::Uint32		CapotScore		= 252;
	static const sf::Uint32		PointsSumScore	= 162;

public:
	BeloteContext(ServerPtr server);
	~BeloteContext();

	void	Reset();

	// Network reactions
	void	HandleGameContextPacket(sf::Packet &packet, ServerSocketPtr sourcePlayer);

	// Players' management
	void	AddPlayer(ServerSocketPtr player);
	void	DropPlayer(ServerSocketPtr player);

private:
	void	SetPlayerPos(ServerSocketPtr player, const std::string &posName);
	void	SendCurrentPositioningToAll();
	void	SendCurrentPositioningTo(ServerSocketPtr player);

	void	NotifyStarting();
	void	NotifyTurnEvent(TurnEvent event, ServerSocketPtr player = ServerSocketPtr());

	void	StartGame();

	void	InitDeck();
	void	ShuffleDeck();
	void	PreTurn();
	void	DealFirstPart();
	void	AskForAsset();
	void	AcceptAsset(const std::string &colourName);
	void	RefuseAsset();
	void	DealLastPart();

	void	StartTurn();
	void	AskToPlay();
	void	CardPlayed(const std::string &card);
	bool	CheckForBelote(const std::string &playedCard);
	void	CheckForRebelote(const std::string &playedCard);
	void	TurnEnded();
	bool	PlayerHasColourInHand(const std::string &colour) const;
	bool	PlayerMustCut() const;
	bool	PlayerCanOvercut() const;
	void	ComputeAndReportTurnScore(PlayerPosition winner);
	void	GameEnded();

	void	EvaluatePlayedCards(Scores &scores) const;
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

	typedef boost::array<std::string, 32>			Deck;
	typedef boost::array<std::string, 8>			PlayerHand;
	typedef boost::array<std::string, _PP_Count>	PlayedCards;

	struct ContextData
	{
		ServerPtr	m_Server;
		Players		m_UnplacedPlayers;
		Players		m_Players;

		sf::Uint32		m_Scores[_TI_Count];
		sf::Uint32		m_LitigeScorePending;
		sf::Uint32		m_TotalScores[_TI_Count];

		Deck						m_Deck;
		int							m_CurrentDeckPos;
		std::vector<std::string>	m_TeamPlayedCards[2];

		PlayerPosition	m_CurrentDealer;
		PlayerPosition	m_CurrentPlayer;

		PlayerHand		m_PlayersHand[_PP_Count];
		int				m_RemainingCards[_PP_Count];

		std::string		m_PotentialAsset;
		bool			m_IsInFirstAnnouncePhase;
		std::string		m_CurrentAsset;
		TeamIndex		m_TeamAcceptingContract;
		TeamIndex		m_TeamOwningBelote;

		int				m_CurrentlyPlayedCards;
		PlayedCards		m_PlayedCards;
	};
	typedef std::shared_ptr<ContextData>	ContextDataPtr;

	// Tools
private:
	static PlayerPosition	GetNextPlayer(PlayerPosition pp)
	{
		if (PP_East == pp)
			return PP_South;
	
		return (PlayerPosition)(pp + 1);
	}

	static TeamIndex		GetOppositeTeam(TeamIndex ti)
	{
		return static_cast<TeamIndex>(ti ^ 1);
	}

	struct IsSameColour : public std::binary_function<std::string, std::string, bool>
	{
		bool operator()(const std::string &testedColour, const std::string &card) const
		{
			return card.size() > 0 && card.front() == testedColour.front();
		}
	};

	struct CardDefToValue : public std::binary_function<ContextDataPtr, std::string, size_t>
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

	struct CardDefToScore : public std::binary_function<ContextDataPtr, std::string, size_t>
	{
		size_t operator()(const ContextDataPtr d, const std::string &card) const
		{
			assert(card.size() > 0);

			if (card.front() == d->m_CurrentAsset.front())
				return AssetScores[ValueOrderAtAsset.rfind(card.c_str() + 1)];

			return NormalScores[ValueOrder.rfind(card.c_str() + 1)];
		}
	};

private:
	ContextDataPtr d;
};

typedef std::shared_ptr<BeloteContext> BeloteContextPtr;

#endif
