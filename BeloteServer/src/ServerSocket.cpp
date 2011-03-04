#include <iostream>

#include "StateMachine.h"

#include "Server.h"
#include "ServerSocket.h"
#include "Packets.h"

// Define network state machine stuff...
enum NetworkEventCodes
{
	NEC_ConnectionRequest,
	NEC_ConnectionAccepted,
};

namespace States
{
	struct ConnectionRequest : public State
	{
		ConnectionRequest(StateMachine *sm) : State(sm), m_Socket(0)	{ ; }

		virtual void	Enter()
		{
			std::cout << "[Server] Accepting connection" << std::endl;

			sf::Packet p;
			p << PT_ConnectionAccepted;
			m_Socket->Send(p);
		}
		
		sf::TcpSocket	*m_Socket;
	};
}

namespace Actions
{
	struct ActionBase : public Action
	{
		ActionBase(sf::TcpSocket &socket) : m_Socket(socket)	{ ; }

		sf::TcpSocket &	m_Socket;
	};

	/*struct Connect : public ActionBase
	{
		Connect(sf::TcpSocket &socket)
			: ActionBase(socket)
		{ ; }

		virtual void operator()()
		{
			m_Socket.Connect(m_HostIP, Server::PORT);
		}

		sf::IpAddress m_HostIP;
	};*/
}

// ServerSocket implementation
class ServerSocketPrivate
{
public:
	ServerSocketPrivate()
		: m_Socket(0), m_StateMachine(0)
	{
		// State machine definition
		m_StateMachine	= new StateMachine;

		m_StateConnectionRequest	= new States::ConnectionRequest(m_StateMachine);
	}

	~ServerSocketPrivate()
	{
		delete m_StateMachine;
	}

	void	SetSocket(sf::TcpSocket *socket)	{ m_Socket = socket; m_StateConnectionRequest->m_Socket = socket; }

	void	Start()								{ m_StateMachine->Start(m_StateConnectionRequest); }
	void	Update()							{ if (!m_StateMachine->IsStopped()) m_StateMachine->Update(); }

private:
	sf::TcpSocket	* m_Socket;

	StateMachine	* m_StateMachine;

	States::ConnectionRequest	* m_StateConnectionRequest;
};

ServerSocket::ServerSocket()
{
	m_priv = new ServerSocketPrivate();
}

ServerSocket::~ServerSocket()
{
	delete m_priv;
}

bool ServerSocket::CheckConnection(sf::TcpListener &listener)
{
	if (listener.Accept(m_Socket) != sf::Socket::Done)
		return false;

	m_Socket.SetBlocking(false);
	m_priv->SetSocket(&m_Socket);
	m_priv->Start();

	return true;
}

void ServerSocket::Update()
{
	m_priv->Update();
}
