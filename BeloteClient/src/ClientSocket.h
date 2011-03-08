#ifndef BELOTE_CLIENTSOCKET_H
#define BELOTE_CLIENTSOCKET_H

#include <string>
#include <queue>

#include <SFML/System.hpp>

#include <CEGUIEventArgs.h>
#include <CEGUIEventSet.h>

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
	static const CEGUI::String EventTextBroadcasted;

public:
	ClientSocket();
	~ClientSocket();

	void	Connect(const std::string &hostIP, const  char *utf8EncodedName);
	void	SendChatMessage(const std::string &utf8EncodedMessage);
	void	EnqueueBroadcastedText(const TextBroadcastedEventArgs &args);
	void	Disconnect();

	void	Update();

private:
	ClientSocketPrivate *					m_priv;
	
	sf::Mutex								m_TextBroadcastedQueueMutex;
	std::queue<TextBroadcastedEventArgs>	m_TextBroadcastedQueue;
};

#endif
