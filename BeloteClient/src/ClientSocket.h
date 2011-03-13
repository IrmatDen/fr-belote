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

class ClientSocketPrivate;

class ClientSocket : public CEGUI::EventSet
{
public:
	static const CEGUI::String EventNamespace;
	static const CEGUI::String EventConnectionStatusUpdated;
	static const CEGUI::String EventPlayerConnected;
	static const CEGUI::String EventPlayerDisconnected;
	static const CEGUI::String EventTextBroadcasted;
	static const CEGUI::String EventCurrentPositioningSent;

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

	// Reserved for private use.
	void	EnqueuePlayerConnected(const PlayerConnectedEventArgs &args);
	// Reserved for private use.
	void	EnqueueBroadcastedText(const TextBroadcastedEventArgs &args);
	// Reserved for private use.
	void	SetCurrentPositioningArgs(const CurrentPositioningArgs &args);
	// Reserved for private use.
	void	SetConnectionStatusArgs(const ConnectionStatusEventArgs &args);

private:
	template <typename T>
	void	UpdateMessageQueue(std::queue<T> &msgQueue, sf::Mutex &mutex,  const CEGUI::String &eventName)
	{
		sf::Lock lock(mutex);
		while (!msgQueue.empty())
		{
			T args = msgQueue.front();
			msgQueue.pop();

			fireEvent(eventName, args, EventNamespace);
		}
	}

private:
	ClientSocketPrivate *					m_priv;

	bool									m_IsDisconnecting;
	
	sf::Mutex								m_TextBroadcastedQueueMutex;
	std::queue<TextBroadcastedEventArgs>	m_TextBroadcastedQueue;
	
	sf::Mutex								m_PlayerConnectedQueueMutex;
	std::queue<PlayerConnectedEventArgs>	m_PlayerConnectedQueue;

	bool									m_IsConnectionStatusReady;
	ConnectionStatusEventArgs				m_ConnectionStateEventArgs;
	
	bool									m_AreCurrentPositioningArgsAvailable;
	CurrentPositioningArgs					m_CurrentPositioningArgs;
};

#endif
