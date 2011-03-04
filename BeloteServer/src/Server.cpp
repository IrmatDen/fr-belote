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
	m_Listener.SetBlocking(false);
	m_Listener.Listen(PORT);

	while(true)
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
	}
}

void Server::Stop()
{
	m_Listener.Close();
}
