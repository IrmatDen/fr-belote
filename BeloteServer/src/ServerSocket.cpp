#include <iostream>
#include <algorithm>
#include <vector>

#include "StateMachine.h"

#include "Server.h"
#include "ServerSocket.h"
#include "Packets.h"

// Define network state machine stuff...
namespace
{
	enum NetworkEventCodes
	{
		NEC_WaitingName,
		NEC_NameReceived,
		NEC_DisconnectionRequest,
		NEC_BroadcastTextRequest,
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
			WaitingName(StateMachine *sm, ServerSocket *ss) : Base(sm), m_ServerSocket(ss)	{ ; }

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
						std::string name;
						p >> name;
						m_ServerSocket->SetClientName(name);
						std::cout << "[Server] Everybody say welcome to " << name << std::endl;
						m_StateMachine->Notify(NEC_NameReceived);
					}
					else
						std::cout << "[Server] Unexpected packet received in State::WaitingName::Update. Packet type is: " << pt << std::endl;
				}
			}

			ServerSocket	* m_ServerSocket;
		};

		struct Idle : public Base
		{
			Idle(StateMachine *sm, Server *server, ServerSocket *ss)
				: Base(sm), m_Server(server), m_ServerSocket(ss)
			{ ; }

			virtual void	Enter()
			{
				std::cout << "[Server] Start procastinating" << std::endl;
			}

			virtual void	Update()
			{
				sf::Packet packet;
				sf::Socket::Status s = m_Socket->Receive(packet);

				if (s == sf::Socket::Done)
				{
					PacketType pt;
					packet >> pt;

					switch (pt)
					{
					case PT_ClientLeave:
						std::cout << "[Server] Everybody say byebye to " << m_ServerSocket->GetClientName() << std::endl;
						m_StateMachine->Stop();
						break;
						
					case PT_ClientTextMessage:
						{
							std::string uft8EncodedMsg;
							packet >> uft8EncodedMsg;
							m_Server->BroadcastText(m_ServerSocket->GetClientName(), uft8EncodedMsg);
						}
						break;

					default:
						std::cout << "[Server] Unexpected packet received in State::Idle::Update. Packet type is: " << pt << std::endl;
						break;
					};
				}
			}

			Server			* m_Server;
			ServerSocket	* m_ServerSocket;
		};

		struct Disconnected : public State
		{
			Disconnected(StateMachine *sm) : State(sm)	{ ; }

			virtual void	Enter()		{ m_StateMachine->Stop(); }
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

		struct BroadcastTextMessage : public Base
		{
			virtual void operator()()
			{
				sf::Packet p;
				p << PT_ServerBroadcastTextMessage << m_ClientName.c_str() << m_Message.c_str();
				m_Socket->Send(p);
			}

			std::string		m_ClientName,
							m_Message;
		};

		struct Disconnect : public Base
		{
			virtual void operator()()
			{
				sf::Packet p;
				p << PT_ServerShuttingDown;
				m_Socket->Send(p);

				std::cout << "[Server] Disconnected " << m_ClientName << std::endl;
			}

			std::string		m_ClientName;
		};
	}
}

// ServerSocket implementation
class ServerSocketPrivate
{
public:
	ServerSocketPrivate(Server *server, ServerSocket *serverSocket)
		: m_Socket(0), m_StateMachine(0), m_Server(server), m_ServerSocket(serverSocket)
	{
		// State machine definition
		m_StateMachine	= new StateMachine;

		// States
		m_StateConnectionRequest	= new States::ConnectionRequest(m_StateMachine);
		m_StateWaitingName			= new States::WaitingName(m_StateMachine, m_ServerSocket);
		m_StateIdle					= new States::Idle(m_StateMachine, m_Server, m_ServerSocket);
		m_StateDisconnected			= new States::Disconnected(m_StateMachine);

		m_StatesWithSocket.push_back((States::Base*)m_StateWaitingName);
		m_StatesWithSocket.push_back((States::Base*)m_StateIdle);

		// Actions
		m_ActionAcceptConnection	= new Actions::AcceptConnection();
		m_ActionDisconnect			= new Actions::Disconnect();
		m_ActionBroadcastText		= new Actions::BroadcastTextMessage();

		m_ActionsWithSocket.push_back((Actions::Base*)m_ActionAcceptConnection);
		m_ActionsWithSocket.push_back(m_ActionDisconnect);
		m_ActionsWithSocket.push_back(m_ActionBroadcastText);

		// Transitions
		m_StateConnectionRequest->AddTransition	(NEC_WaitingName,			m_StateWaitingName,		m_ActionAcceptConnection);
		m_StateWaitingName->AddTransition		(NEC_NameReceived,			m_StateIdle										);
		m_StateIdle->AddTransition				(NEC_BroadcastTextRequest,	m_StateIdle,			m_ActionBroadcastText	);
		m_StateIdle->AddTransition				(NEC_DisconnectionRequest,	m_StateDisconnected,	m_ActionDisconnect		);
	}

	~ServerSocketPrivate()
	{
		delete m_StateMachine;
	}

	void SetSocket(sf::TcpSocket *socket)
	{
		m_Socket = socket;
		std::for_each(m_StatesWithSocket.begin(), m_StatesWithSocket.end(),
				[this](States::Base *sb) { sb->m_Socket = m_Socket; });
		std::for_each(m_ActionsWithSocket.begin(), m_ActionsWithSocket.end(),
				[this](Actions::Base *ab) { ab->m_Socket = m_Socket; });
	}

	void Start()
	{
		m_StateMachine->Start(m_StateConnectionRequest);
	}

	void Update()
	{
		if (!m_StateMachine->IsStopped())
			m_StateMachine->Update();
		else // free the slot
			m_Socket->Disconnect();
	}

	void SendText(const std::string &clientName, const std::string &msg)
	{
		m_ActionBroadcastText->m_ClientName = clientName;
		m_ActionBroadcastText->m_Message = msg;
		m_StateMachine->Notify(NEC_BroadcastTextRequest);
	}

	void Abort()
	{
		if (!m_Socket)
			return;

		if (!m_StateMachine->IsStopped())
		{
			m_ActionDisconnect->m_ClientName = m_ServerSocket->GetClientName();
			m_StateMachine->Notify(NEC_DisconnectionRequest);
			m_StateMachine->Update();
		}

		m_Socket->Disconnect();
		m_Socket = 0;
	}

private:
	Server			* m_Server;
	ServerSocket	* m_ServerSocket;
	sf::TcpSocket	* m_Socket;

	StateMachine	* m_StateMachine;

	std::vector<States::Base*>	m_StatesWithSocket;
	State		* m_StateConnectionRequest,
				* m_StateWaitingName,
				* m_StateIdle,
				* m_StateDisconnected;
	
	std::vector<Actions::Base*>		m_ActionsWithSocket;
	Action							* m_ActionAcceptConnection;
	Actions::Disconnect				* m_ActionDisconnect;
	Actions::BroadcastTextMessage	* m_ActionBroadcastText;
};

ServerSocket::ServerSocket(Server *server)
{
	m_priv = new ServerSocketPrivate(server, this);
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

void ServerSocket::SendText(const std::string &clientName, const std::string &msg)
{
	m_priv->SendText(clientName, msg);
}

void ServerSocket::CloseConnection()
{
	// We want to be sure that the clients will get his disconnection message.
	m_Socket.SetBlocking(true);
	m_priv->Abort();
}
