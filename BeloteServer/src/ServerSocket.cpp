#include <iostream>
#include <algorithm>
#include <vector>

#include "StateMachine.h"

#include "Server.h"
#include "Packets.h"
#include "ServerSocket.h"
#include "BeloteContext.h"

// Define network state machine stuff...
namespace
{
	enum NetworkEventCodes
	{
		NEC_WaitingName,
		NEC_NameReceived,
		NEC_DisconnectionRequest,
		NEC_BroadcastTextRequest,
		NEC_BroadcastClientConnected,
		NEC_BroadcastClientDisconnected,
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
				sf::Packet packet;
				sf::Socket::Status s = m_Socket->Receive(packet);

				if (s == sf::Socket::Done)
				{
					PacketType pt;
					packet >> pt;

					if (pt == PT_ClientName)
					{
						std::string name;
						packet >> name;
						m_ServerSocket->SetClientName(name);
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
						{
							m_Server->ClientDisconnected(m_ServerSocket->GetClientName());
							m_StateMachine->Stop();
							
							BeloteContext *context = m_ServerSocket->GetBeloteContext();
							context->DropPlayer(m_ServerSocket);
						}
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
			AcceptConnection(Server *server) : m_Server(server)		{ ; }
			virtual void operator()()
			{
				sf::Packet p;
				if (m_Server->GetClientCount() < Server::MAX_CLIENTS_ON_TABLE)
					p << PT_ConnectionAccepted;
				else
					p << PT_ConnectionDeniedLobbyFull;
				m_Socket->Send(p);
			}
			
			Server			* m_Server;
		};
		
		struct NotifyClientConnected : public Base
		{
			NotifyClientConnected(Server *server, ServerSocket *ss) : m_Server(server), m_ServerSocket(ss) { ; }

			virtual void operator()()
			{
				m_Server->ClientConnected(m_ServerSocket->GetClientName());
				
				BeloteContext *context = m_ServerSocket->GetBeloteContext();
				context->AddPlayer(m_ServerSocket);
			}
			
			Server			* m_Server;
			ServerSocket	* m_ServerSocket;
		};

		struct BroadcastClientConnected : public Base
		{
			virtual void operator()()
			{
				sf::Packet p;
				p << PT_ClientConnected << m_ClientName.c_str();
				m_Socket->Send(p);
			}

			std::string		m_ClientName;
		};

		struct BroadcastClientDisconnected : public Base
		{
			virtual void operator()()
			{
				sf::Packet p;
				p << PT_ClientDisconnected << m_ClientName.c_str();
				m_Socket->Send(p);
			}

			std::string		m_ClientName;
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
		m_ActionAcceptConnection			= new Actions::AcceptConnection(m_Server);
		m_ActionDisconnect					= new Actions::Disconnect();
		m_ActionNotifyClientConnected		= new Actions::NotifyClientConnected(m_Server, m_ServerSocket);
		m_ActionBroadcastClientConnected	= new Actions::BroadcastClientConnected();
		m_ActionBroadcastClientDisconnected	= new Actions::BroadcastClientDisconnected();
		m_ActionBroadcastText				= new Actions::BroadcastTextMessage();

		m_ActionsWithSocket.push_back((Actions::Base*)m_ActionAcceptConnection);
		m_ActionsWithSocket.push_back(m_ActionDisconnect);
		m_ActionsWithSocket.push_back(m_ActionBroadcastClientConnected);
		m_ActionsWithSocket.push_back(m_ActionBroadcastClientDisconnected);
		m_ActionsWithSocket.push_back(m_ActionBroadcastText);

		// Transitions
		m_StateConnectionRequest->AddTransition(NEC_WaitingName,					m_StateWaitingName,		m_ActionAcceptConnection			);
		m_StateWaitingName		->AddTransition(NEC_NameReceived,					m_StateIdle,			m_ActionNotifyClientConnected		);
		m_StateIdle				->AddTransition(NEC_BroadcastClientConnected,		m_StateIdle,			m_ActionBroadcastClientConnected	);
		m_StateIdle				->AddTransition(NEC_BroadcastClientDisconnected,	m_StateIdle,			m_ActionBroadcastClientDisconnected	);
		m_StateIdle				->AddTransition(NEC_BroadcastTextRequest,			m_StateIdle,			m_ActionBroadcastText				);
		m_StateIdle				->AddTransition(NEC_DisconnectionRequest,			m_StateDisconnected,	m_ActionDisconnect					);
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

	void ClientConnected(const std::string &clientName)
	{
		m_ActionBroadcastClientConnected->m_ClientName = clientName;
		m_StateMachine->Notify(NEC_BroadcastClientConnected);
	}

	void ClientDisconnected(const std::string &clientName)
	{
		m_ActionBroadcastClientDisconnected->m_ClientName = clientName;
		m_StateMachine->Notify(NEC_BroadcastClientDisconnected);
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
	
	std::vector<Actions::Base*>			m_ActionsWithSocket;
	Action									* m_ActionAcceptConnection;
	Actions::Disconnect						* m_ActionDisconnect;
	Action									* m_ActionNotifyClientConnected;
	Actions::BroadcastClientConnected		* m_ActionBroadcastClientConnected;
	Actions::BroadcastClientDisconnected	* m_ActionBroadcastClientDisconnected;
	Actions::BroadcastTextMessage			* m_ActionBroadcastText;
};

ServerSocket::ServerSocket(Server *server, BeloteContext *beloteContext)
	: m_BeloteContext(beloteContext)
{
	m_priv = new ServerSocketPrivate(server, this);
}

ServerSocket::~ServerSocket()
{
	delete m_priv;
}

bool ServerSocket::CheckConnection(sf::TcpListener &listener)
{
	if (IsConnected())
		return true;

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

void ServerSocket::ClientConnected(const std::string &clientName)
{
	m_priv->ClientConnected(clientName);
}

void ServerSocket::ClientDisconnected(const std::string &clientName)
{
	m_priv->ClientDisconnected(clientName);
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
