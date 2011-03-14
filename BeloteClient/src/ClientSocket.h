#ifndef BELOTE_CLIENTSOCKET_H
#define BELOTE_CLIENTSOCKET_H

#include <string>
#include <queue>

#include <SFML/System.hpp>

#include <CEGUIEventArgs.h>
#include <CEGUIEventSet.h>

class ConnectionStatusEventArgs : public CEGUI::EventArgs
{
public:
	enum ConnectionStatus
	{
		CS_Connected,
		CS_Disconnected,
		CS_LobbyFull
	};

	ConnectionStatus	m_ConnectionStatus;
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

class CurrentPositioningArgs : public CEGUI::EventArgs
{
public:
	std::string		m_Pos[4];
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

class ClientSocketPrivate;

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

class ClientSocket : public CEGUI::EventSet
{
public:
	static const CEGUI::String EventNamespace;
	static const CEGUI::String EventConnectionStatusUpdated;
	static const CEGUI::String EventPlayerConnected;
	static const CEGUI::String EventTextBroadcasted;
	static const CEGUI::String EventCurrentPositioningSent;
	static const CEGUI::String EventGameStarting;
	static const CEGUI::String EventCardsReceived;
	static const CEGUI::String EventPotentialAsset;

public:
	ThreadSafeEventQueue<PlayerConnectedEventArgs>		m_PlayerConnected;
	ThreadSafeEventQueue<TextBroadcastedEventArgs>		m_TextBroadcasted;
	ThreadSafeEventQueue<CurrentCardsInHandArgs>		m_CardsReceived;
	ThreadSafeEventQueue<CurrentPositioningArgs>		m_CurrentPositioningSent;
	ThreadSafeEventQueue<>								m_GameStarting;
	ThreadSafeEventQueue<ConnectionStatusEventArgs>		m_ConnectionStatus;
	ThreadSafeEventQueue<PotentialAssetArgs>			m_PotentialAsset;

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
			return currentQueue.back().m_ConnectionStatus == ConnectionStatusEventArgs::CS_Connected;
		}

	} m_ConnectionStatusPushGuard;

public:
	ClientSocket();
	~ClientSocket();

	void	Connect(const std::string &hostIP, const std::string &utf8EncodedName);
	void	SendChatMessage(const std::string &utf8EncodedMessage);
	void	Disconnect();

	//! This will fire any events queued by the socket's thread.
	void	Update();

	//! This will wait until the socket has finished running (must only be used when quitting).
	void	Wait();

	void	ChoosePosition(const std::string &posName);
	void	StartGame();

private:
	ClientSocketPrivate *	m_priv;
	bool					m_IsDisconnecting;
};

#endif
