#ifndef BELOTESERVER_SERVERSOCKET_H
#define BELOTESERVER_SERVERSOCKET_H

#include <SFML\Network.hpp>

class Server;
class BeloteContext;
class ServerSocketPrivate;

class ServerSocket
{
public:
	ServerSocket(Server *server, BeloteContext *beloteContext);
	~ServerSocket();

	bool	CheckConnection(sf::TcpListener &listener);
	bool	IsConnected() const								{ return m_Socket.GetRemoteAddress() != sf::IpAddress::None; }
	void	Update();
	void	ClientDisconnected(const std::string &clientName);
	void	ClientConnected(const std::string &clientName);
	void	SendText(const std::string &clientName, const std::string &msg);
	void	CloseConnection();

	void				SetClientName(const std::string &clientName)	{ m_ClientName =  clientName; }
	const std::string&	GetClientName() const							{ return m_ClientName; }

	BeloteContext*		GetBeloteContext()								{ return m_BeloteContext; }

private:
	ServerSocketPrivate		* m_priv;
	BeloteContext			* m_BeloteContext;
	sf::TcpSocket			m_Socket;
	std::string				m_ClientName;
};

#endif
