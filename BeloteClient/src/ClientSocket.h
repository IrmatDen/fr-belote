#ifndef BELOTE_CLIENTSOCKET_H
#define BELOTE_CLIENTSOCKET_H

#include <string>

class ClientSocketPrivate;

class ClientSocket
{
public:
	ClientSocket();

	void	Connect(const std::string &hostIP, const char *utf8EncodedName);
	void	Disconnect();

private:
	ClientSocketPrivate *	m_priv;
};

#endif
