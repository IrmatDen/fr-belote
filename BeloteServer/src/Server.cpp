#include <iostream>

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "../include/Server.h"

Server::Server()
{
}

void Server::Start()
{
	m_Listener.Listen(PORT);

	while(true)
	{
		sf::TcpSocket socket;
		if (m_Listener.Accept(socket) == sf::Socket::Done)
		{
			std::cout << "New client, yay! {" << socket.GetRemoteAddress().ToString() << "}" << std::endl;
		}
	}
}

void Server::Stop()
{
	m_Listener.Close();
}
