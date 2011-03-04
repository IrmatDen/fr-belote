#ifndef BELOTESERVER_SERVERSOCKET_H
#define BELOTESERVER_SERVERSOCKET_H

#include <SFML\Network.hpp>

class ServerSocketPrivate;

class ServerSocket
{
public:
	ServerSocket();
	~ServerSocket();

	bool	CheckConnection(sf::TcpListener &listener);
	bool	IsConnected() const								{ return m_Socket.GetRemoteAddress() != sf::IpAddress::None; }
	void	Update();

private:
	ServerSocketPrivate *	m_priv;
	sf::TcpSocket			m_Socket;
};

#endif
