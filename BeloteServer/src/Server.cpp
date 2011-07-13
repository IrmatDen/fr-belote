#include <iostream>
#include <algorithm>

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "Server.h"
#include "Packets.h"
#include "ServerSocket.h"

#include "BeloteContext.h"
#include "BeloteContextTypes.h"

Server::Server()
	: m_Running(false)
{
	m_BeloteContext = BeloteContextPtr(new BeloteContext(shared_from_this()));

	m_Clients.resize(MAX_CLIENTS);
	std::generate(m_Clients.begin(), m_Clients.end(),
			[&] () -> Clients::value_type
			{ return ServerSocketPtr(new ServerSocket(shared_from_this(), m_BeloteContext)); }
		);
}

Server::~Server()
{
}

void Server::Start()
{
	m_Running = true;

	m_Listener.SetBlocking(false);
	m_Listener.Listen(PORT);

	m_BeloteContext->Reset();
}

void Server::Update()
{
	if (!m_Running)
		return;

	m_ClientsCount = 0;

	std::for_each(m_Clients.begin(), m_Clients.end(),
			[&] (Clients::reference ref)
			{	if (ref->IsConnected())
				{
					ref->Update();
					m_ClientsCount++;
				}
				ref->CheckConnection(m_Listener);
			}
		);

	m_BeloteContext->Update();
}

void Server::SetRuleSet(BeloteContextRuleSet &ruleSet)
{
	m_BeloteContext->SetRuleSet(ruleSet);
}

void Server::Stop()
{
	if (!m_Running)
		return;

	// Notify clients that the server is shutting down
	std::for_each(m_Clients.begin(), m_Clients.end(),
			[] (Clients::reference ref)
			{ if (ref->IsConnected()) ref->CloseConnection(); }
		);

	m_Listener.Close();

	m_BeloteContext->Reset();

	m_Running = false;
}

void Server::ClientConnected(const std::string &clientName)
{
	std::for_each(m_Clients.begin(), m_Clients.end(),
			[&] (Clients::reference ref)
			{ if (ref->IsConnected()) ref->ClientConnected(clientName); }
		);
}

void Server::ClientDisconnected(const std::string &clientName)
{
	std::for_each(m_Clients.begin(), m_Clients.end(),
			[&] (Clients::reference ref)
			{ if (ref->IsConnected()) ref->ClientDisconnected(clientName); }
		);
}

void Server::BroadcastText(const std::string &clientName, const std::string &msg)
{
	std::for_each(m_Clients.begin(), m_Clients.end(),
			[&] (Clients::reference ref)
			{ if (ref->IsConnected()) ref->SendText(clientName, msg); }
		);
}

void Server::BroadcastSystemMessage(const std::string &msg)
{
	std::for_each(m_Clients.begin(), m_Clients.end(),
			[&] (Clients::reference ref)
			{ if (ref->IsConnected()) ref->SendSystemMessage(msg); }
		);
}
