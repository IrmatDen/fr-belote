#ifndef BELOTESERVER_SERVERSOCKET_H
#define BELOTESERVER_SERVERSOCKET_H

#include <SFML\Network.hpp>

#include "typedefs.h"

class Server;
class BeloteContext;

class ServerSocketPrivate;
typedef std::shared_ptr<ServerSocketPrivate> ServerSocketPrivatePtr;

class ServerSocket : public std::enable_shared_from_this<ServerSocket>
{
public:
	ServerSocket(std::shared_ptr<Server> server, std::shared_ptr<BeloteContext> beloteContext);
	~ServerSocket();

	bool	CheckConnection(sf::TcpListener &listener);
	bool	IsConnected() const								{ return m_Socket.GetRemoteAddress() != sf::IpAddress::None; }
	void	Update();
	void	ClientDisconnected(const std::string &clientName);
	void	ClientConnected(const std::string &clientName);
	void	SendText(const std::string &clientName, const std::string &msg);
	void	SendSystemMessage(const std::string &msg);
	void	CloseConnection();

	void				SetClientName(const std::string &clientName)	{ m_ClientName =  clientName; }
	const std::string&	GetClientName() const							{ return m_ClientName; }

	std::shared_ptr<BeloteContext>	GetBeloteContext()					{ return m_BeloteContext; }
	sf::TcpSocket&					GetSocket()							{ return m_Socket; }

private:
	ServerSocketPrivatePtr				m_priv;
	std::shared_ptr<BeloteContext>	m_BeloteContext;
	sf::TcpSocket						m_Socket;
	std::string							m_ClientName;
};
typedef std::shared_ptr<ServerSocket>	ServerSocketPtr;

#endif
