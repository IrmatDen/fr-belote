#ifndef BELOTESERVER_SERVER_H
#define BELOTESERVER_SERVER_H

#include <SFML/Network.hpp>

class Client;

class Server
{
public:
	static const unsigned short		PORT	= 45000;

public:
	Server();

	void	Start();
	void	Stop();

private:
	static const unsigned int	MAX_CLIENTS = 4;

private:
	sf::TcpListener		m_Listener;
	Client *			m_Clients[MAX_CLIENTS];
};

#endif
