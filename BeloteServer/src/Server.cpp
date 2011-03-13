#include <iostream>
#include <algorithm>

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "Server.h"
#include "Packets.h"
#include "ServerSocket.h"

#include "BeloteContext.h"

Server::Server()
{
	m_BeloteContext = new BeloteContext();

	m_Clients.resize(MAX_CLIENTS);
	std::generate(m_Clients.begin(), m_Clients.end(),
			[&] () -> Clients::value_type
			{ return new ServerSocket(this, m_BeloteContext); }
		);
}

Server::~Server()
{
	std::for_each(m_Clients.begin(), m_Clients.end(), [] (Clients::reference ref) { delete ref; } );
}

void Server::Start()
{
	m_Running = true;

	m_Listener.SetBlocking(false);
	m_Listener.Listen(PORT);

	m_BeloteContext->Reset();

	while(m_Running)
	{
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

		sf::Sleep(0.005f);
	}
	
	// Notify clients that the server is shutting down
	std::for_each(m_Clients.begin(), m_Clients.end(),
			[] (Clients::reference ref)
			{ if (ref->IsConnected()) ref->CloseConnection(); }
		);

	m_Listener.Close();
}

void Server::Stop()
{
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
