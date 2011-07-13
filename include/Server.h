#ifndef BELOTESERVER_SERVER_H
#define BELOTESERVER_SERVER_H

#include <SFML/Network.hpp>

class ServerSocket;
typedef std::shared_ptr<ServerSocket> ServerSocketPtr;

class BeloteContext;
typedef std::shared_ptr<BeloteContext> BeloteContextPtr;

struct BeloteContextRuleSet;

class Server : public std::enable_shared_from_this<Server>
{
public:
	static const unsigned short		PORT					= 45000;
	static const unsigned int		MAX_CLIENTS				= 16;
	static const unsigned int		MAX_CLIENTS_ON_TABLE	= 4;

public:
	Server();
	~Server();

	unsigned int	GetClientCount() const		{ return m_ClientsCount; }

	void	Start();
	void	Update();
	void	Stop();

	void	SetRuleSet(BeloteContextRuleSet &ruleSet);
	void	ClientConnected(const std::string &clientName);
	void	ClientDisconnected(const std::string &clientName);
	void	BroadcastText(const std::string &clientName, const std::string &msg);
	void	BroadcastSystemMessage(const std::string &msg);

private:
	typedef std::vector<ServerSocketPtr>	Clients;
	typedef Clients::iterator				ClientsIt;
	typedef Clients::const_iterator			ClientsConstIt;

	bool				m_Running;
	sf::TcpListener		m_Listener;
	Clients				m_Clients;
	unsigned int		m_ClientsCount;

	BeloteContextPtr	m_BeloteContext;
};
typedef std::shared_ptr<Server>	ServerPtr;

#endif
