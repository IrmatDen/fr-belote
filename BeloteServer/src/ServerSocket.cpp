#include <iostream>
#include <algorithm>
#include <vector>

#include "StateMachine.h"
#include "typedefs.h"

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
			Base(StateMachinePtr sm) : State(sm)		{ ; }
		
			sf::TcpSocketPtr	m_Socket;
		};
		typedef boost::shared_ptr<Base>	BasePtr;

		struct ConnectionRequest : public Base
		{
			ConnectionRequest(StateMachinePtr sm) : Base(sm)	{ ; }

			virtual void	Enter()
			{
				std::cout << "[Server] Accepting connection" << std::endl;
				m_StateMachine->Notify(NEC_WaitingName);
			}
		};

		struct WaitingName : public Base
		{
			WaitingName(StateMachinePtr sm, ServerSocketPtr ss) : Base(sm), m_ServerSocket(ss)	{ ; }

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

			ServerSocketPtr m_ServerSocket;
		};

		struct Idle : public Base
		{
			Idle(StateMachinePtr sm, ServerPtr server, ServerSocketPtr ss)
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
							
							m_ServerSocket->GetBeloteContext()->DropPlayer(m_ServerSocket);
						}
						break;
						
					case PT_ClientTextMessage:
						{
							std::string uft8EncodedMsg;
							packet >> uft8EncodedMsg;
							m_Server->BroadcastText(m_ServerSocket->GetClientName(), uft8EncodedMsg);
						}
						break;

					case PT_GameContextPacket:
						m_ServerSocket->GetBeloteContext()->HandleGameContextPacket(packet, m_ServerSocket);
						break;

					default:
						std::cout << "[Server] Unexpected packet received in State::Idle::Update. Packet type is: " << pt << std::endl;
						break;
					};
				}
			}

			ServerPtr		m_Server;
			ServerSocketPtr	m_ServerSocket;
		};

		struct Disconnected : public Base
		{
			Disconnected(StateMachinePtr sm) : Base(sm)	{ ; }

			virtual void	Enter()		{ m_StateMachine->Stop(); }
		};
	}

	namespace Actions
	{
		struct Base : public Action
		{
			Base()				{ ; }

			sf::TcpSocketPtr	m_Socket;
		};
		typedef boost::shared_ptr<Base>	BasePtr;

		struct AcceptConnection : public Base
		{
			AcceptConnection(ServerPtr server) : m_Server(server)		{ ; }
			virtual void operator()()
			{
				sf::Packet p;
				if (m_Server->GetClientCount() < Server::MAX_CLIENTS_ON_TABLE)
					p << PT_ConnectionAccepted;
				else
					p << PT_ConnectionDeniedLobbyFull;
				m_Socket->Send(p);
			}
			
			ServerPtr m_Server;
		};
		
		struct NotifyClientConnected : public Base
		{
			NotifyClientConnected(ServerPtr server, ServerSocketPtr ss) : m_Server(server), m_ServerSocket(ss) { ; }

			virtual void operator()()
			{
				m_Server->ClientConnected(m_ServerSocket->GetClientName());
				
				m_ServerSocket->GetBeloteContext()->AddPlayer(m_ServerSocket);
			}
			
			ServerPtr		m_Server;
			ServerSocketPtr	m_ServerSocket;
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
		typedef boost::shared_ptr<BroadcastClientConnected>	BroadcastClientConnectedPtr;

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
		typedef boost::shared_ptr<BroadcastClientDisconnected>	BroadcastClientDisconnectedPtr;

		struct BroadcastTextMessage : public Base
		{
			virtual void operator()()
			{
				sf::Packet p;
				if (m_ClientName != "")
					p << PT_ServerBroadcastTextMessage << m_ClientName.c_str() << m_Message.c_str();
				else
					p << PT_ServerBroadcastSystemMessage << m_Message;
				m_Socket->Send(p);
			}

			std::string		m_ClientName,
							m_Message;
		};
		typedef boost::shared_ptr<BroadcastTextMessage>	BroadcastTextMessagePtr;

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
		typedef boost::shared_ptr<Disconnect>	DisconnectPtr;
	}
}

// ServerSocket implementation
class ServerSocketPrivate
{
public:
	ServerSocketPrivate(ServerPtr server, ServerSocketPtr serverSocket)
		: m_Server(server), m_ServerSocket(serverSocket)
	{
		// State machine definition
		m_StateMachine	= StateMachinePtr(new StateMachine);

		// States
		m_StateConnectionRequest	= States::BasePtr(new States::ConnectionRequest(m_StateMachine));
		m_StateWaitingName			= States::BasePtr(new States::WaitingName(m_StateMachine, m_ServerSocket));
		m_StateIdle					= States::BasePtr(new States::Idle(m_StateMachine, m_Server, m_ServerSocket));
		m_StateDisconnected			= States::BasePtr(new States::Disconnected(m_StateMachine));

		m_StatesWithSocket.push_back(m_StateWaitingName);
		m_StatesWithSocket.push_back(m_StateIdle);

		// Actions
		m_ActionAcceptConnection			= Actions::BasePtr							(new Actions::AcceptConnection(m_Server));
		m_ActionDisconnect					= Actions::DisconnectPtr					(new Actions::Disconnect());
		m_ActionNotifyClientConnected		= Actions::BasePtr							(new Actions::NotifyClientConnected(m_Server, m_ServerSocket));
		m_ActionBroadcastClientConnected	= Actions::BroadcastClientConnectedPtr		(new Actions::BroadcastClientConnected());
		m_ActionBroadcastClientDisconnected	= Actions::BroadcastClientDisconnectedPtr	(new Actions::BroadcastClientDisconnected());
		m_ActionBroadcastText				= Actions::BroadcastTextMessagePtr			(new Actions::BroadcastTextMessage());

		m_ActionsWithSocket.push_back(m_ActionAcceptConnection);
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

	void SetSocket(sf::TcpSocketPtr socket)
	{
		m_Socket = socket;
		std::for_each(m_StatesWithSocket.begin(), m_StatesWithSocket.end(),
				[this](States::BasePtr sb) { sb->m_Socket = m_Socket; });
		std::for_each(m_ActionsWithSocket.begin(), m_ActionsWithSocket.end(),
				[this](Actions::BasePtr ab) { ab->m_Socket = m_Socket; });
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

	void SendSystemMessage(const std::string &msg)
	{
		m_ActionBroadcastText->m_ClientName = "";
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
		m_Socket.reset();
	}

private:
	ServerPtr			m_Server;
	ServerSocketPtr		m_ServerSocket;
	sf::TcpSocketPtr	m_Socket;

	StateMachinePtr	m_StateMachine;

	std::vector<States::BasePtr>	m_StatesWithSocket;
	States::BasePtr	m_StateConnectionRequest,
					m_StateWaitingName,
					m_StateIdle,
					m_StateDisconnected;
	
	std::vector<Actions::BasePtr>			m_ActionsWithSocket;
	Actions::BasePtr						m_ActionAcceptConnection;
	Actions::DisconnectPtr					m_ActionDisconnect;
	Actions::BasePtr						m_ActionNotifyClientConnected;
	Actions::BroadcastClientConnectedPtr	m_ActionBroadcastClientConnected;
	Actions::BroadcastClientDisconnectedPtr	m_ActionBroadcastClientDisconnected;
	Actions::BroadcastTextMessagePtr		m_ActionBroadcastText;
};

ServerSocket::ServerSocket(ServerPtr server, BeloteContextPtr beloteContext)
	: m_BeloteContext(beloteContext)
{
	m_Socket	= sf::TcpSocketPtr(new sf::TcpSocket);
	m_priv		= ServerSocketPrivatePtr(new ServerSocketPrivate(server, ServerSocketPtr(this)));
}

ServerSocket::~ServerSocket()
{
}

bool ServerSocket::CheckConnection(sf::TcpListener &listener)
{
	if (IsConnected())
		return true;

	if (listener.Accept(*m_Socket) != sf::Socket::Done)
		return false;

	m_Socket->SetBlocking(false);
	m_priv->SetSocket(m_Socket);
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

void ServerSocket::SendSystemMessage(const std::string &msg)
{
	m_priv->SendSystemMessage(msg);
}

void ServerSocket::CloseConnection()
{
	// We want to be sure that the clients will get his disconnection message.
	m_Socket->SetBlocking(true);
	m_priv->Abort();
}
