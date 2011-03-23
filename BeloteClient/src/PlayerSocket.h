#ifndef BELOTECLIENT_PLAYERSOCKET_H
#define BELOTECLIENT_PLAYERSOCKET_H

#include <CEGUIEventArgs.h>
#include <CEGUIEventSet.h>

#include "ClientSocket.h"

class ConnectionStatusEventArgs : public CEGUI::EventArgs
{
public:
	ClientSocket::ConnectionStatus	m_ConnectionStatus;
};

class PlayerConnectedEventArgs : public CEGUI::EventArgs
{
public:
	bool		m_Connected;
	std::string	m_PlayerName;
};

class TextBroadcastedEventArgs : public CEGUI::EventArgs
{
public:
	std::string	m_Teller,
				m_Message;
};

class SystemMessageBroadcastedEventArgs : public CEGUI::EventArgs
{
public:
	std::string	m_Message;
};

class CurrentPositioningArgs : public CEGUI::EventArgs
{
public:
	std::string		m_Pos[4];
};

class PlayerDealingArgs : public CEGUI::EventArgs
{
public:
	std::string	m_Who;
};

class PlayerRefusedAssetArgs : public CEGUI::EventArgs
{
public:
	int				m_PlayerPos;
};

class PlayerAcceptedAssetArgs : public CEGUI::EventArgs
{
public:
	int				m_PlayerPos;
	std::string		m_Asset;
	bool			m_AcceptedByNSTeam;
};

class CurrentCardsInHandArgs : public CEGUI::EventArgs
{
public:
	std::string		m_Cards[8];
};

class PotentialAssetArgs : public CEGUI::EventArgs
{
public:
	std::string		m_Card;
};

class WaitingPlayArgs : public CEGUI::EventArgs
{
public:
	std::string		m_PossibleCards[8];
};

class PlayedCardArgs : public CEGUI::EventArgs
{
public:
	std::string		m_Card;
	int				m_Player;
};

class ScoresArgs : public CEGUI::EventArgs
{
public:
	int		m_NorthSouthScore,
			m_WestEastScore;
};

class BeloteAnnouncedArgs : public CEGUI::EventArgs
{
public:
	int		m_ByPlayerPos;
};

class ContractingTeamResultArgs : public CEGUI::EventArgs
{
public:
	bool	m_IsNorthSouthTeam,
			m_HasWon;
};

class LitigeArgs : public CEGUI::EventArgs
{
public:
	int	m_LitigeValue;
};

class MatchWonArgs : public CEGUI::EventArgs
{
public:
	bool	m_MatchWonByNSTeam;
};

template <typename ArgType = CEGUI::EventArgs>
class ThreadSafeEventQueue
{
public:
	typedef std::queue<ArgType>	EventQueue;

public:
	ThreadSafeEventQueue(const CEGUI::String &eventName, const CEGUI::String &eventNamespace)
		: m_EventName(eventName), m_EventNamespace(eventNamespace)
	{
	}

	void	reset()
	{
		std::swap(m_EventQueue, EventQueue());
	}
	
	void	push(const ArgType &arg = ArgType())
	{
		sf::Lock l(m_Mutex);
		m_EventQueue.push(arg);
	}

	// guard is expected to be a callable object of type:
	// bool (guard)(const EventQueue &currentQueue)
	// such that the arguments gets pushed only if the return value is true.
	template <typename GuardType>
	void	push(GuardType guard, const ArgType &arg = ArgType())
	{
		sf::Lock l(m_Mutex);
		if (guard(m_EventQueue))
			m_EventQueue.push(arg);
	}

	void	process(CEGUI::EventSet *eventSet)
	{
		sf::Lock lock(m_Mutex);
		while (!m_EventQueue.empty())
		{
			ArgType args = m_EventQueue.front();
			m_EventQueue.pop();

			eventSet->fireEvent(m_EventName, args, m_EventNamespace);
		}
	}

private:
	sf::Mutex		m_Mutex;
	EventQueue		m_EventQueue;
	CEGUI::String	m_EventName;
	CEGUI::String	m_EventNamespace;
};

class PlayerSocket : public CEGUI::EventSet, public ClientSocket
{
public:
	static const CEGUI::String EventNamespace;
	static const CEGUI::String EventConnectionStatusUpdated;
	static const CEGUI::String EventPlayerConnected;
	static const CEGUI::String EventTextBroadcasted;
	static const CEGUI::String EventSystemMessageBroadcasted;
	static const CEGUI::String EventCurrentPositioningSent;
	static const CEGUI::String EventGameStarting;
	static const CEGUI::String EventPlayerDealing;
	static const CEGUI::String EventCardsReceived;
	static const CEGUI::String EventPotentialAsset;
	static const CEGUI::String EventAskRevealedAsset;
	static const CEGUI::String EventAskAnotherAsset;
	static const CEGUI::String EventPlayerRefusedAsset;
	static const CEGUI::String EventPlayerAcceptedAsset;
	static const CEGUI::String EventTurnStarting;
	static const CEGUI::String EventWaitingPlay;
	static const CEGUI::String EventPlayedCard;
	static const CEGUI::String EventCurrentScores;
	static const CEGUI::String EventTotalScores;
	static const CEGUI::String EventBeloteAnnounced;
	static const CEGUI::String EventRebeloteAnnounced;
	static const CEGUI::String EventNoAssetTaken;
	static const CEGUI::String EventContractingTeamResult;
	static const CEGUI::String EventLitige;
	static const CEGUI::String EventMatchWon;

public:
	PlayerSocket();

	void	Update();

	virtual void	OnConnectionStatusChanged(ConnectionStatus newStatus);
	virtual void	OnPlayerConnected(const std::string &playerName);
	virtual void	OnPlayerDisconnected(const std::string &playerName);
	virtual void	OnTextBroadcasted(const std::string &sayer, const std::string &msg);
	virtual void	OnSysMsgBroadcasted(const std::string &msg);
	virtual void	OnPositionningReceived(const PositionningPacket &positionning);
	virtual void	OnGameStarting();
	virtual void	OnPlayerDealing(const std::string &dealerName);
	virtual void	OnCardsDealt(const CardsDealtPacket &cards);
	virtual void	OnPotentialAssetReceived(const std::string &assetCard);
	virtual void	OnAskingRevealedAsset();
	virtual void	OnAskingAnotherAsset();
	virtual void	OnAcceptedAsset(const AcceptedAssetPacket &acceptedAsset);
	virtual void	OnRefusedAsset(int refusingPlayerPos);
	virtual void	OnTurnStarting();
	virtual void	OnWaitingPlay(const WaitingPlayPacket &waitingPlay);
	virtual void	OnPlayedCard(const PlayedCardPacket &playedCard);
	virtual void	OnCurrentScores(int NSScore, int WEScore);
	virtual void	OnTotalScores(int NSScore, int WEScore);
	virtual void	OnBeloteAnnounced(int announcingPos);
	virtual void	OnRebeloteAnnounced(int announcingPos);
	virtual void	OnNoAssetTaken();
	virtual void	OnContractingTeamResult(bool isNSTeamContracting, bool hasWon);
	virtual void	OnLitige(int litigeValue);
	virtual void	OnMatchWon(bool isWonByNSTeam);

private:
	ThreadSafeEventQueue<PlayerConnectedEventArgs>			m_PlayerConnected;
	ThreadSafeEventQueue<TextBroadcastedEventArgs>			m_TextBroadcasted;
	ThreadSafeEventQueue<SystemMessageBroadcastedEventArgs>	m_SystemMessageBroadcasted;
	ThreadSafeEventQueue<CurrentCardsInHandArgs>			m_CardsReceived;
	ThreadSafeEventQueue<CurrentPositioningArgs>			m_CurrentPositioningSent;
	ThreadSafeEventQueue<>									m_GameStarting;
	ThreadSafeEventQueue<PlayerDealingArgs>					m_PlayerDealing;
	ThreadSafeEventQueue<ConnectionStatusEventArgs>			m_ConnectionStatus;
	ThreadSafeEventQueue<PotentialAssetArgs>				m_PotentialAsset;
	ThreadSafeEventQueue<>									m_AskRevealedAsset;
	ThreadSafeEventQueue<>									m_AskAnotherAsset;
	ThreadSafeEventQueue<PlayerRefusedAssetArgs>			m_PlayerRefusedAsset;
	ThreadSafeEventQueue<PlayerAcceptedAssetArgs>			m_PlayerAcceptedAsset;
	ThreadSafeEventQueue<>									m_TurnStarting;
	ThreadSafeEventQueue<WaitingPlayArgs>					m_WaitingPlay;
	ThreadSafeEventQueue<PlayedCardArgs>					m_PlayedCard;
	ThreadSafeEventQueue<ScoresArgs>						m_CurrentScores;
	ThreadSafeEventQueue<ScoresArgs>						m_TotalScores;
	ThreadSafeEventQueue<BeloteAnnouncedArgs>				m_BeloteAnnounced;
	ThreadSafeEventQueue<BeloteAnnouncedArgs>				m_RebeloteAnnounced;
	ThreadSafeEventQueue<>									m_NoAssetTaken;
	ThreadSafeEventQueue<ContractingTeamResultArgs>			m_ContractingTeamResult;
	ThreadSafeEventQueue<LitigeArgs>						m_Litige;
	ThreadSafeEventQueue<MatchWonArgs>						m_MatchWon;

	struct ConnectionStatusPushGuard
		: public std::unary_function<
				const ThreadSafeEventQueue<ConnectionStatusEventArgs>::EventQueue&,
				bool
			>
	{
		typedef ThreadSafeEventQueue<ConnectionStatusEventArgs>::EventQueue	EventQueue;
		bool operator()(const EventQueue &currentQueue)
		{
			if (currentQueue.size() == 0)
				return true;
			return currentQueue.back().m_ConnectionStatus == ClientSocket::CS_Connected;
		}

	} m_ConnectionStatusPushGuard;
};

#endif