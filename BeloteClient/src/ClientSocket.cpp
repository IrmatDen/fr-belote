#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "Packets.h"
#include "Server.h"

#include "ClientSocket.h"
#include "StateMachine.h"

// Define network state machine stuff...
namespace
{
	enum NetworkEventCodes
	{
		NEC_ConnectionRequest,
		NEC_Connected,
		NEC_SendName,
		NEC_CantConnect,
		NEC_SendTextMessage,
		NEC_DisconnectionRequest,
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
				{
					m_StateMachine->Notify(NEC_CantConnect);
					return;
				}
				// If the connection is made, we'll just have to wait for feedback from the server before continuing.
			}

			virtual void	Update()
			{
				sf::Packet p;
				sf::Socket::Status s = m_Socket.Receive(p);

				if (s == sf::Socket::Done)
				{
					PacketType pt;
					p >> pt;

					if (pt == PT_ConnectionAccepted)
					{
						std::cout << "[Client] Connection accepted, yay!" << std::endl;
						m_StateMachine->Notify(NEC_Connected);
					}
					else
						std::cout << "[Client] Unexpected packet received in State::Connecting::Update. Packet type is: " << pt << std::endl;
				}
			}

			sf::TcpSocket &m_Socket;
		};

		struct Connected : public State
		{
			Connected(StateMachine *sm)
				: State(sm)
			{ ; }

			virtual void	Enter()
			{
				m_StateMachine->Notify(NEC_SendName);
			}
		};

		struct Idle : public State
		{
			Idle(StateMachine *sm, sf::TcpSocket &socket)
				: State(sm), m_Socket(socket)
			{ ; }

			virtual void	Update()
			{
				m_Socket.SetBlocking(false);
				sf::Packet packet;
				sf::Socket::Status s = m_Socket.Receive(packet);
				m_Socket.SetBlocking(true);

				if (s == sf::Socket::Done)
				{
					PacketType pt;
					packet >> pt;

					switch (pt)
					{
					case PT_ServerShuttingDown:
						std::cout << "[Client] Server shutting down, me too" << std::endl;
						m_StateMachine->Notify(NEC_DisconnectionRequest);
						break;

					case PT_ServerBroadcastTextMessage:
						{
							std::string uft8EncodedSayer, uft8EncodedMsg;
							packet >> uft8EncodedSayer >> uft8EncodedMsg;
							std::cout << "[Client] Server broadcast msg from " << uft8EncodedSayer << " saying: " << uft8EncodedMsg << std::endl;
							//m_StateMachine->Notify(NEC_DisconnectionRequest);
						}
						break;

					default:
						std::cout << "[Client] Unexpected packet received in State::Idle::Update. Packet type is: " << pt << std::endl;
						break;
					}
				}
			}

			sf::TcpSocket &m_Socket;
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

		struct SendName : public ActionBase
		{
			SendName(sf::TcpSocket &socket)
				: ActionBase(socket)
			{ ; }

			virtual void operator()()
			{
				sf::Packet p;
				p << PT_ClientName << m_Utf8EncodedName;
				sf::Socket::Status s = m_Socket.Send(p);

				// Error checking
				if (s != sf::Socket::Done)
					std::cout << "[Client] Error sending name in Actions::SendName. Error code: " << s << std::endl;
			}

			const char		*m_Utf8EncodedName;
		};

		struct SendTextMessage : public ActionBase
		{
			SendTextMessage(sf::TcpSocket &socket)
				: ActionBase(socket)
			{ ; }

			virtual void operator()()
			{
				sf::Packet p;
				p << PT_ClientTextMessage << m_Utf8EncodedText.c_str();
				sf::Socket::Status s = m_Socket.Send(p);

				// Error checking
				if (s != sf::Socket::Done)
					std::cout << "[Client] Error sending message in Actions::SendTextMessage. Error code: " << s << std::endl;
			}

			std::string		m_Utf8EncodedText;
		};

		struct Disconnect : public ActionBase
		{
			Disconnect(sf::TcpSocket &socket) : ActionBase(socket)	{ ; }
			virtual void operator()()
			{
				sf::Packet p;
				p << PT_ClientLeave;
				m_Socket.Send(p);
			
				m_Socket.Disconnect();
				std::cout << "[Client] Disconnected" << std::endl;
			}
		};
	}
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
		
		// States
		m_StateWfc			= new States::WaitingForConnection(m_StateMachine);
		m_StateConnecting	= new States::Connecting(m_StateMachine, m_Socket);
		m_StateConnected	= new States::Connected(m_StateMachine);
		m_StateIdle			= new States::Idle(m_StateMachine, m_Socket);
		m_StateDisconnected	= new States::Disconnected(m_StateMachine);

		// Actions
		m_ActionConnect		= new Actions::Connect(m_Socket);
		m_ActionSendName	= new Actions::SendName(m_Socket);
		m_ActionSendTxtMsg	= new Actions::SendTextMessage(m_Socket);
		m_ActionDisconnect	= new Actions::Disconnect(m_Socket);
		
		// Transitions
		m_StateWfc->AddTransition		(NEC_ConnectionRequest,		m_StateConnecting,		m_ActionConnect		);
		m_StateConnecting->AddTransition(NEC_Connected,				m_StateConnected							);
		m_StateConnecting->AddTransition(NEC_CantConnect,			m_StateDisconnected							);
		m_StateConnected->AddTransition	(NEC_SendName,				m_StateIdle,			m_ActionSendName	);
		m_StateIdle->AddTransition		(NEC_SendTextMessage,		m_StateIdle,			m_ActionSendTxtMsg	);
		m_StateIdle->AddTransition		(NEC_DisconnectionRequest,	m_StateDisconnected,	m_ActionDisconnect	);
	}

	void	Connect(const std::string &hostIP, const char *utf8EncodedName)
	{
		m_DisconnectRequested				= false;
		m_ActionConnect->m_HostIP			= sf::IpAddress(hostIP);
		m_ActionSendName->m_Utf8EncodedName	= utf8EncodedName;

		m_Thread->Launch();
	}

	void	Disconnect()
	{
		m_DisconnectRequested = true;
	}

	void	SendChatMessage(const std::string &utf8EncodedMessage)
	{
		m_ActionSendTxtMsg->m_Utf8EncodedText = utf8EncodedMessage;
		m_StateMachine->Notify(NEC_SendTextMessage);
	}

private:
	void	ThreadEP()
	{
		m_StateMachine->Start(m_StateWfc);

		while (!m_StateMachine->IsStopped())
		{
			m_StateMachine->Update();

			// If the client decided to leave the table only! When the server kicks the player, this is handled by states.
			if (m_DisconnectRequested)
				m_StateMachine->Notify(NEC_DisconnectionRequest);

			sf::Sleep(0.05f);
		}

		int n = 42;
	}

private:
	sf::IpAddress	m_HostIP;
	sf::TcpSocket	m_Socket;
	sf::Thread *	m_Thread;

	// Flags
	bool			m_DisconnectRequested;

	// State machine
	StateMachine	* m_StateMachine;

	State		* m_StateWfc,
				* m_StateConnecting,
				* m_StateConnected,
				* m_StateIdle,
				* m_StateDisconnected;
	
	Actions::Connect			* m_ActionConnect;
	Actions::SendName			* m_ActionSendName;
	Actions::SendTextMessage	* m_ActionSendTxtMsg;
	Action						* m_ActionDisconnect;
};

ClientSocket::ClientSocket()
	: m_priv(new ClientSocketPrivate)
{
}

ClientSocket::~ClientSocket()
{
	delete m_priv;
}

void ClientSocket::Connect(const std::string &hostIP, const  char *utf8EncodedName)
{
	m_priv->Connect(hostIP, utf8EncodedName);
}

void ClientSocket::Disconnect()
{
	m_priv->Disconnect();
}

void ClientSocket::SendChatMessage(const std::string &utf8EncodedMessage)
{
	m_priv->SendChatMessage(utf8EncodedMessage);
}
