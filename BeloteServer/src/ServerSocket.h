#ifndef BELOTESERVER_SERVERSOCKET_H
#define BELOTESERVER_SERVERSOCKET_H

#include <SFML\Network.hpp>

class Server;
class ServerSocketPrivate;

class ServerSocket
{
public:
	ServerSocket(Server *server);
	~ServerSocket();

	bool	CheckConnection(sf::TcpListener &listener);
	bool	IsConnected() const								{ return m_Socket.GetRemoteAddress() != sf::IpAddress::None; }
	void	Update();
	void	SendText(const std::string &clientName, const std::string &msg);
	void	CloseConnection();

	void				SetClientName(const std::string &clientName)	{ m_ClientName =  clientName; }
	const std::string&	GetClientName() const							{ return m_ClientName; }

private:
	ServerSocketPrivate		* m_priv;
	sf::TcpSocket			m_Socket;
	std::string				m_ClientName;
};

#endif
