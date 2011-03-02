#ifndef BELOTE_CLIENTSOCKET_H
#define BELOTE_CLIENTSOCKET_H

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "StateMachine.h"

class ClientSocketPrivate;

class ClientSocket
{
public:
	ClientSocket();

	void	Connect(const std::string &hostIP);

private:
	ClientSocketPrivate *	m_priv;
};

#endif
