#include <iostream>

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "../include/Server.h"
#include "ServerSocket.h"

Server::Server()
{
	for (int i = 0; i != MAX_CLIENTS; i++)
		m_Clients[i] = new ServerSocket(this);
}

void Server::Start()
{
	m_Running = true;

	m_Listener.SetBlocking(false);
	m_Listener.Listen(PORT);

	while(m_Running)
	{
		for (int i = 0; i != MAX_CLIENTS; i++)
		{
			if (m_Clients[i]->IsConnected())
			{
				m_Clients[i]->Update();
				continue;
			}

			m_Clients[i]->CheckConnection(m_Listener);
		}

		// FIXME srsly...
		sf::Sleep(0.005f);
	}
	
	// Notify clients that the server is shutting down
	for (int i = 0; i != MAX_CLIENTS; i++)
		m_Clients[i]->CloseConnection();

	m_Listener.Close();
}

void Server::Stop()
{
	m_Running = false;
}

void Server::ClientConnected(const std::string &clientName)
{
	for (int i = 0; i != MAX_CLIENTS; i++)
	{
		if (!m_Clients[i]->IsConnected())
			continue;

		m_Clients[i]->ClientConnected(clientName);
	}
}

void Server::BroadcastText(const std::string &clientName, const std::string &msg)
{
	for (int i = 0; i != MAX_CLIENTS; i++)
	{
		if (!m_Clients[i]->IsConnected())
			continue;

		m_Clients[i]->SendText(clientName, msg);
	}
}
