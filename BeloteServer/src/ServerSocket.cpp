#include <iostream>
#include <algorithm>
#include <vector>

#include "StateMachine.h"

#include "Server.h"
#include "ServerSocket.h"
#include "Packets.h"

// Define network state machine stuff...
enum NetworkEventCodes
{
	NEC_ConnectionAccepted,
	NEC_NameReceived,
};

namespace States
{
	struct Base : public State
	{
		Base(StateMachine *sm) : State(sm), m_Socket(0)		{ ; }
		
		sf::TcpSocket	*m_Socket;
	};

	struct ConnectionRequest : public Base
	{
		ConnectionRequest(StateMachine *sm) : Base(sm)	{ ; }

		virtual void	Enter()
		{
			std::cout << "[Server] Accepting connection" << std::endl;

			sf::Packet p;
			p << PT_ConnectionAccepted;
			m_Socket->Send(p);

			m_StateMachine->Notify(NEC_ConnectionAccepted);
		}
	};

	struct WaitingName : public Base
	{
		WaitingName(StateMachine *sm) : Base(sm)	{ ; }

		virtual void	Update()
		{
			sf::Packet p;
			sf::Socket::Status s = m_Socket->Receive(p);

			if (s == sf::Socket::Done)
			{
				PacketType pt;
				p >> pt;

				if (pt == PT_ClientName)
				{
					std::string n;
					p >> n;
					std::cout << "[Server] Everybody say welcome to " << n << std::endl;
					m_StateMachine->Notify(NEC_NameReceived);
				}
				else
					std::cout << "[Server] Unexpected packet received in State::WaitingName::Update. Packet type is: " << pt << std::endl;
			}
		}
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

		// States
		m_StateConnectionRequest = new States::ConnectionRequest(m_StateMachine);
		m_StatesWithSocket.push_back((States::Base*)m_StateConnectionRequest);

		m_WaitingName = new States::WaitingName(m_StateMachine);
		m_StatesWithSocket.push_back((States::Base*)m_WaitingName);

		// Transitions
		m_StateConnectionRequest->AddTransition(NEC_ConnectionAccepted, m_WaitingName);
	}

	~ServerSocketPrivate()
	{
		delete m_StateMachine;
	}

	void	SetSocket(sf::TcpSocket *socket)
	{
		m_Socket = socket;
		std::for_each(m_StatesWithSocket.begin(), m_StatesWithSocket.end(),
				[this](States::Base *sb) { sb->m_Socket = m_Socket; });
	}

	void	Start()								{ m_StateMachine->Start(m_StateConnectionRequest); }
	void	Update()							{ if (!m_StateMachine->IsStopped()) m_StateMachine->Update(); }

private:
	sf::TcpSocket	* m_Socket;

	StateMachine	* m_StateMachine;

	std::vector<States::Base*>	m_StatesWithSocket;
	State	* m_StateConnectionRequest,
			* m_WaitingName;
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
