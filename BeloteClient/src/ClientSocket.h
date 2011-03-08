#ifndef BELOTE_CLIENTSOCKET_H
#define BELOTE_CLIENTSOCKET_H

#include <string>

class ClientSocketPrivate;

class ClientSocket
{
public:
	ClientSocket();
	~ClientSocket();

	void	Connect(const std::string &hostIP, const  char *utf8EncodedName);
	void	SendChatMessage(const std::string &utf8EncodedMessage);
	void	Disconnect();

private:
	ClientSocketPrivate *	m_priv;
};

#endif
