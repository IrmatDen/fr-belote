#ifndef BELOTESERVER_CLIENT_H
#define BELOTESERVER_CLIENT_H

#include <SFML\Network.hpp>

class Client
{
public:
	Client()		{ ; }

	bool			CheckConnection(sf::TcpListener &listener)		{ return listener.Accept(m_Socket) == sf::Socket::Done; }
	bool			IsConnected() const								{ return m_Socket.GetRemoteAddress() != sf::IpAddress::None; }

private:
	sf::TcpSocket	m_Socket;
};

#endif
