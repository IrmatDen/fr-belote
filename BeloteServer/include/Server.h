#ifndef BELOTE_SERVER_H
#define BELOTE_SERVER_H

#include <SFML/Network/TcpListener.hpp>

class Server
{
public:
	static const unsigned short		PORT	= 45000;

public:
	Server();

	void	Start();
	void	Stop();

private:
	sf::TcpListener		m_Listener;
};

#endif
