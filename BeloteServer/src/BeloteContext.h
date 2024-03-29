#ifndef BELOTESERVER_BELOTECONTEXT_H
#define BELOTESERVER_BELOTECONTEXT_H

#include <string>
#include <vector>

#include <boost/array.hpp>

#include <SFML/Config.hpp>
#include <SFML/Network.hpp>

#include "ServerSocket.h"
#include "BeloteContextPackets.h"
#include "BeloteContextTypes.h"
#include "IASocket.h"

class Server;
typedef std::shared_ptr<Server> ServerPtr;

class BeloteContext
{
public:
	typedef boost::array<std::string, 4> StringArray4;
	static const StringArray4	PlayerPositionStrings;

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
		TE_MatchEnded,
	};

	static const sf::Uint32		LastTurnScore	= 10;
	static const sf::Uint32		BeloteScore		= 20;
	static const sf::Uint32		MinScoreToWin	= 82;
	static const sf::Uint32		CapotScore		= 252;
	static const sf::Uint32		PointsSumScore	= 162;

public:
	BeloteContext(ServerPtr server);
	~BeloteContext();
	
	void	SetRuleSet(BeloteContextRuleSet &ruleSet)		{ d->m_RuleSet = ruleSet; }
	void	Reset();
	void	Update();

	// Network reactions
	void	HandleGameContextPacket(sf::Packet &packet, ServerSocketPtr sourcePlayer);

	// Players' management
	void	AddPlayer(ServerSocketPtr player);
	void	DropPlayer(ServerSocketPtr player);

private:
	void	SetPlayerPos(ServerSocketPtr player, const std::string &posName);
	void	UnseatPlayer(ServerSocketPtr player);
	void	SendCurrentPositioningToAll();
	void	SendCurrentPositioningTo(ServerSocketPtr player);
	void	SpawnBots();

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

	void		StartTurn();
	void		AskToPlay();
	void		CardPlayed(const std::string &card);
	bool		CheckForBelote(const std::string &playedCard);
	void		CheckForRebelote(const std::string &playedCard);
	void		TurnEnded();
	bool		PlayerHasColourInHand(const std::string &colour) const;
	bool		PlayerMustCut() const;
	bool		PlayerCanOvercut() const;
	void		ComputeAndReportTurnScore(PlayerPosition winner);
	void		GameEnded();
	bool		CheckMatchEnd() const;
	TeamIndex	GetMatchWinningTeam() const;

	void	EvaluatePlayedCards(Scores &scores) const;
	size_t	GetMaxScoreFromPlayedCards() const;
	void	DumpAllCardsInHandTo(std::vector<std::string> &out) const;
	void	DumpColoursInHandTo(const std::string &colour, std::vector<std::string> &out) const;
	void	DumpOvercuttingCardsTo(std::vector<std::string> &out) const;

	void	OrderHands();
	void	SendCurrentHands();

private:
	typedef std::vector<IASocketPtr>		Bots;

	typedef std::vector<ServerSocketPtr>	Players;
	typedef Players::iterator				PlayersIt;
	typedef Players::const_iterator			PlayersConstIt;

	typedef boost::array<std::string, 32>			Deck;
	typedef boost::array<std::string, _PP_Count>	PlayedCards;

	struct ContextData
	{
		ServerPtr	m_Server;
		Players		m_UnplacedPlayers;
		Players		m_Players;

		BeloteContextRuleSet	m_RuleSet;

		size_t		m_RequiredBotsCount;
		Bots		m_Bots;

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
	static PlayerPosition	GetNextPlayer(PlayDirection pd, PlayerPosition pp)
	{
		if (pd == PD_CW)
		{
			if (PP_East == pp)
				return PP_South;
	
			return (PlayerPosition)(pp + 1);
		}

		// CCW
		if (PP_South == pp)
			return PP_East;
	
		return (PlayerPosition)(pp - 1);
	}
	
	static PlayerPosition	GetWinnerPosition(PlayDirection pd, PlayerPosition endingPos, size_t winnerPosFromSP)
	{
		if (pd == PD_CW)
		{
			const PlayerPosition startingPos = GetNextPlayer(pd, endingPos);
			return static_cast<PlayerPosition>((startingPos + winnerPosFromSP) % _PP_Count);
		}

		// CCW
		const PlayerPosition startingPos = GetNextPlayer(pd, endingPos);
		return static_cast<PlayerPosition>(_PP_Count - startingPos - winnerPosFromSP);
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

private:
	ContextDataPtr d;
};

typedef std::shared_ptr<BeloteContext> BeloteContextPtr;

#endif
