#ifndef BELOTE_CLIENTSOCKET_H
#define BELOTE_CLIENTSOCKET_H

#include <string>
#include <queue>

#include <SFML/System.hpp>

#include <CEGUIEventArgs.h>
#include <CEGUIEventSet.h>

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

class ClientSocketPrivate;

class ClientSocket : public CEGUI::EventSet
{
public:
	static const CEGUI::String EventNamespace;
	static const CEGUI::String EventPlayerConnected;
	static const CEGUI::String EventPlayerDisconnected;
	static const CEGUI::String EventTextBroadcasted;

public:
	ClientSocket();
	~ClientSocket();

	void	Connect(const std::string &hostIP, const std::string &utf8EncodedName);
	void	SendChatMessage(const std::string &utf8EncodedMessage);
	void	EnqueuePlayerConnected(const PlayerConnectedEventArgs &args);
	void	EnqueueBroadcastedText(const TextBroadcastedEventArgs &args);
	void	Disconnect();

	void	Update();

	//! This will wait until the socket has finished running (must only be used when quitting).
	void	Wait();

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
	
	sf::Mutex								m_TextBroadcastedQueueMutex;
	std::queue<TextBroadcastedEventArgs>	m_TextBroadcastedQueue;
	
	sf::Mutex								m_PlayerConnectedQueueMutex;
	std::queue<PlayerConnectedEventArgs>	m_PlayerConnectedQueue;
};

#endif
