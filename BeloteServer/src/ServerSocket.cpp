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
	NEC_WaitingName,
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
			m_StateMachine->Notify(NEC_WaitingName);
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

	struct Idle : public Base
	{
		Idle(StateMachine *sm) : Base(sm)	{ ; }

		virtual void	Enter()
		{
			std::cout << "[Server] Start procastinating" << std::endl;
		}

		virtual void	Update()
		{
			sf::Packet p;
			sf::Socket::Status s = m_Socket->Receive(p);

			if (s == sf::Socket::Done)
			{
				PacketType pt;
				p >> pt;

				if (pt == PT_ClientLeave)
				{
					std::cout << "[Server] Everybody say byebye to <someone>" << std::endl;
					m_StateMachine->Stop();
				}
				else
					std::cout << "[Server] Unexpected packet received in State::Idle::Update. Packet type is: " << pt << std::endl;
			}
		}
	};
}

namespace Actions
{
	struct Base : public Action
	{
		Base() : m_Socket(0)	{ ; }

		sf::TcpSocket *	m_Socket;
	};

	struct AcceptConnection : public Base
	{
		virtual void operator()()
		{
			sf::Packet p;
			p << PT_ConnectionAccepted;
			m_Socket->Send(p);
		}
	};
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
		m_StateConnectionRequest	= new States::ConnectionRequest(m_StateMachine);
		m_StateWaitingName			= new States::WaitingName(m_StateMachine);
		m_StateIdle					= new States::Idle(m_StateMachine);

		m_StatesWithSocket.push_back((States::Base*)m_StateWaitingName);
		m_StatesWithSocket.push_back((States::Base*)m_StateIdle);

		// Actions
		m_ActionAcceptConnection	= new Actions::AcceptConnection();

		m_ActionsWithSocket.push_back((Actions::Base*)m_ActionAcceptConnection);

		// Transitions
		m_StateConnectionRequest->AddTransition	(NEC_WaitingName,		m_StateWaitingName,		m_ActionAcceptConnection);
		m_StateWaitingName->AddTransition		(NEC_NameReceived,		m_StateIdle										);
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
		std::for_each(m_ActionsWithSocket.begin(), m_ActionsWithSocket.end(),
				[this](Actions::Base *ab) { ab->m_Socket = m_Socket; });
	}

	void	Start()								{ m_StateMachine->Start(m_StateConnectionRequest); }
	void	Update()
	{
		if (!m_StateMachine->IsStopped())
			m_StateMachine->Update();
		else // free the slot
			m_Socket->Disconnect();
	}

private:
	sf::TcpSocket	* m_Socket;

	StateMachine	* m_StateMachine;

	std::vector<States::Base*>	m_StatesWithSocket;
	State		* m_StateConnectionRequest,
				* m_StateWaitingName,
				* m_StateIdle;
	
	std::vector<Actions::Base*>	m_ActionsWithSocket;
	Action		* m_ActionAcceptConnection;
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
