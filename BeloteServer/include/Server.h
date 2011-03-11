#ifndef BELOTESERVER_SERVER_H
#define BELOTESERVER_SERVER_H

#include <SFML/Network.hpp>

class ServerSocket;

class Server
{
public:
	static const unsigned short		PORT					= 45000;
	static const unsigned int		MAX_CLIENTS				= 16;
	static const unsigned int		MAX_CLIENTS_ON_TABLE	= 4;

public:
	Server();

	unsigned int	GetClientCount() const		{ return m_ClientsCount; }

	void	Start();
	void	ClientConnected(const std::string &clientName);
	void	ClientDisconnected(const std::string &clientName);
	void	BroadcastText(const std::string &clientName, const std::string &msg);
	void	Stop();

private:
	bool				m_Running;
	unsigned int		m_ClientsCount;
	sf::TcpListener		m_Listener;
	ServerSocket *		m_Clients[MAX_CLIENTS];
};

#endif
