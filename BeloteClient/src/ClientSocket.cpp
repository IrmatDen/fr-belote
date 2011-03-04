#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "Server.h"

#include "ClientSocket.h"
#include "StateMachine.h"

// Define network state machine stuff...
enum NetworkEventCodes
{
	NEC_ConnectionRequest,
	NEC_Connected,
	NEC_CantConnect,
	NEC_DisconnectionRequest
};

namespace States
{
	struct WaitingForConnection : public State
	{
		WaitingForConnection(StateMachine *sm) : State(sm)	{ ; }

		virtual void	Enter()		{ m_StateMachine->Notify(NEC_ConnectionRequest); }
	};
	
	struct Connecting : public State
	{
		Connecting(StateMachine *sm, sf::TcpSocket &socket)
			: State(sm), m_Socket(socket)
		{ ; }

		virtual void	Enter()
		{
			if (m_Socket.GetRemoteAddress() == sf::IpAddress::None)
				m_StateMachine->Notify(NEC_CantConnect);
			else
				m_StateMachine->Notify(NEC_Connected);
		}

		sf::TcpSocket &m_Socket;
	};

	struct Connected : public State
	{
		Connected(StateMachine *sm) : State(sm)	{ ; }

		virtual void	Enter()		{ /*m_StateMachine->Notify(NEC_DisconnectionRequest);*/ }
	};

	struct Disconnected : public State
	{
		Disconnected(StateMachine *sm) : State(sm)	{ ; }

		virtual void	Enter()		{ m_StateMachine->Stop(); }
	};
}

namespace Actions
{
	struct ActionBase : public Action
	{
		ActionBase(sf::TcpSocket &socket) : m_Socket(socket)	{ ; }

		sf::TcpSocket &	m_Socket;
	};

	struct Connect : public ActionBase
	{
		Connect(sf::TcpSocket &socket)
			: ActionBase(socket)
		{ ; }

		virtual void operator()()
		{
			m_Socket.Connect(m_HostIP, Server::PORT);
		}

		sf::IpAddress m_HostIP;
	};

	struct Disconnect : public ActionBase
	{
		Disconnect(sf::TcpSocket &socket) : ActionBase(socket)	{ ; }
		virtual void operator()()
		{
			m_Socket.Disconnect();
			std::cout << "Disconnected" << std::endl;
		}
	};
}

// ClientSocket implementation

class ClientSocketPrivate
{
public:
	ClientSocketPrivate()
		: m_Thread(0)
	{
		m_Thread = new sf::Thread(&ClientSocketPrivate::ThreadEP, this);

		// State machine definition
		m_StateMachine = new StateMachine();

		m_StateWfc			= new States::WaitingForConnection(m_StateMachine);
		m_StateConnecting	= new States::Connecting(m_StateMachine, m_Socket);
		m_StateConnected	= new States::Connected(m_StateMachine);
		m_StateDisconnected	= new States::Disconnected(m_StateMachine);

		m_ActionConnect		= new Actions::Connect(m_Socket);
		m_ActionDisconnect	= new Actions::Disconnect(m_Socket);

		m_StateWfc->AddTransition		(NEC_ConnectionRequest,		m_StateConnecting,		m_ActionConnect		);
		m_StateConnecting->AddTransition(NEC_Connected,				m_StateConnected							);
		m_StateConnecting->AddTransition(NEC_CantConnect,			m_StateDisconnected							);
		m_StateConnected->AddTransition	(NEC_DisconnectionRequest,	m_StateDisconnected,	m_ActionDisconnect	);
	}

	void	Connect(const std::string &hostIP)
	{
		m_ActionConnect->m_HostIP = sf::IpAddress(hostIP);
		m_Thread->Launch();
	}

private:
	void	ThreadEP()
	{
		m_StateMachine->Start(m_StateWfc);

		while (!m_StateMachine->IsStopped())
		{
			m_StateMachine->Update();
		}
	}

private:
	sf::IpAddress	m_HostIP;
	sf::TcpSocket	m_Socket;
	sf::Thread *	m_Thread;

	StateMachine	* m_StateMachine;

	State		* m_StateWfc,
				* m_StateConnecting,
				* m_StateConnected,
				* m_StateDisconnected;
	
	Actions::Connect	* m_ActionConnect;
	Action				* m_ActionDisconnect;
};

ClientSocket::ClientSocket()
	: m_priv(new ClientSocketPrivate)
{
}

void ClientSocket::Connect(const std::string &hostIP)
{
	m_priv->Connect(hostIP);
}
