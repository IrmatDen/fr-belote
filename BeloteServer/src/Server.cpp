#include <iostream>

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "../include/Server.h"
#include "ServerSocket.h"

Server::Server()
{
	for (int i = 0; i != MAX_CLIENTS; i++)
		m_Clients[i] = new ServerSocket;
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
