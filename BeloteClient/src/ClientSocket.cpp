#include <iostream>

#include <Windows.h>

#include <SFML/Network.hpp>

#include "Tools.h"
#include "StateMachine.h"

#include "Packets.h"
#include "BeloteContextPackets.h"
#include "Server.h"

#include "ClientSocket.h"

const CEGUI::String ClientSocket::EventNamespace("ClientSocket");
const CEGUI::String ClientSocket::EventConnectionStatusUpdated("ConnectionStatusUpdated");
const CEGUI::String ClientSocket::EventPlayerConnected("PlayerConnected");
const CEGUI::String ClientSocket::EventPlayerDisconnected("PlayerDisconnected");
const CEGUI::String ClientSocket::EventTextBroadcasted("TextBroadcasted");
const CEGUI::String ClientSocket::EventCurrentPositioningSent("CurrentPositioningSent");
const CEGUI::String ClientSocket::EventGameStarting("GameStarting");
const CEGUI::String ClientSocket::EventCardsReceived("CardsReceived");

// Define network state machine stuff...
namespace
{
	enum NetworkEventCodes
	{
		NEC_ConnectionRequest,
		NEC_Connected,
		NEC_LobbyFull,
		NEC_SendName,
		NEC_CantConnect,
		NEC_SendTextMessage,
		NEC_SendPlayerPosition,
		NEC_StartGame,
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

			virtual void	Update()
			{
				sf::Packet p;
				sf::Socket::Status status = m_Socket.Receive(p);

				if (status == sf::Socket::Done)
				{
					PacketType pt;
					p >> pt;

					if (pt == PT_ConnectionAccepted)
					{
						m_StateMachine->Notify(NEC_Connected);
					}
					else if (pt == PT_ConnectionDeniedLobbyFull)
					{
						std::cout << "[Client] Lobby full, boo!" << std::endl;
						m_StateMachine->Notify(NEC_LobbyFull);
					}
					else
						std::cout << "[Client] Unexpected packet received in State::Connecting::Update. Packet type is: " << pt << std::endl;
				}
				else if (status == sf::Socket::Disconnected)
				{
					m_StateMachine->Notify(NEC_CantConnect);
				}
			}

			sf::TcpSocket &m_Socket;
		};

		struct Connected : public State
		{
			Connected(StateMachine *sm, ClientSocket *self)
				: State(sm), m_Self(self)
			{ ; }

			virtual void	Enter()
			{
				ConnectionStatusEventArgs args;
				args.m_ConnectionStatus = ConnectionStatusEventArgs::CS_Connected;
				m_Self->SetConnectionStatusArgs(args);
				m_StateMachine->Notify(NEC_SendName);
			}

			ClientSocket	* m_Self;
		};

		struct Idle : public State
		{
			Idle(StateMachine *sm, sf::TcpSocket &socket, ClientSocket *self)
				: State(sm), m_Socket(socket), m_Self(self)
			{ ; }

			virtual void	Update()
			{
				sf::Packet packet;
				m_Socket.SetBlocking(false);
				sf::Socket::Status status = m_Socket.Receive(packet);
				m_Socket.SetBlocking(true);

				if (status == sf::Socket::Done)
				{
					PacketType pt;
					packet >> pt;

					switch (pt)
					{
					case PT_ClientConnected:
						{
							PlayerConnectedEventArgs args;
							packet >> args.m_PlayerName;
							args.m_Connected = true;
							m_Self->EnqueuePlayerConnected(args);
						}
						break;
						
					case PT_ClientDisconnected:
						{
							PlayerConnectedEventArgs args;
							packet >> args.m_PlayerName;
							args.m_Connected = false;
							m_Self->EnqueuePlayerConnected(args);
						}
						break;

					case PT_ServerBroadcastTextMessage:
						{
							TextBroadcastedEventArgs args;
							packet >> args.m_Teller >> args.m_Message;
							m_Self->EnqueueBroadcastedText(args);
						}
						break;

					case PT_GameContextPacket:
						HandleGameContextPacket(packet);
						break;

					case PT_ServerShuttingDown:
						std::cout << "[Client] Server shutting down, me too" << std::endl;
						m_StateMachine->Notify(NEC_DisconnectionRequest);
						break;

					default:
						std::cout << "[Client] Unexpected packet received in State::Idle::Update. Packet type is: " << pt << std::endl;
						break;
					}
				}
				else if (status == sf::Socket::Error)
				{
					m_StateMachine->Notify(NEC_CantConnect);
				}
			}

			sf::TcpSocket	& m_Socket;
			ClientSocket	* m_Self;

		private:
			void HandleGameContextPacket(sf::Packet &packet)
			{
				BeloteContextPacketType bcpt;
				packet >> bcpt;

				switch(bcpt)
				{
				case BCPT_CurrentPositionning:
					{
						CurrentPositioningArgs args;
						for (sf::Uint32 i = 0; i != countof(args.m_Pos); i++)
							packet >> args.m_Pos[i];
						m_Self->SetCurrentPositioningArgs(args);
					}
					break;
					
				case BCPT_GameStarting:
					{
						m_Self->SetGameStarting();
					}
					break;

				case BCPT_CardsDealt:
					{
						CurrentCardsInHandArgs args;
						for (sf::Uint32 i = 0; i != countof(args.m_Cards); i++)
							packet >> args.m_Cards[i];
						m_Self->SetCardsInHandArgs(args);
					}
					break;

				default:
					break;
				}
			}
		};

		struct LobbyFull : public State
		{
			LobbyFull(StateMachine *sm, ClientSocket *self)
				: State(sm), m_Self(self)
			{ ; }

			virtual void	Enter()
			{
				ConnectionStatusEventArgs args;
				args.m_ConnectionStatus = ConnectionStatusEventArgs::CS_LobbyFull;
				m_Self->SetConnectionStatusArgs(args);

				m_StateMachine->Notify(NEC_DisconnectionRequest);
			}

			ClientSocket	* m_Self;
		};

		struct Disconnected : public State
		{
			Disconnected(StateMachine *sm, ClientSocket *self)
				: State(sm), m_Self(self)
			{ ; }

			virtual void	Enter()
			{
				ConnectionStatusEventArgs args;
				args.m_ConnectionStatus = ConnectionStatusEventArgs::CS_Disconnected;
				m_Self->SetConnectionStatusArgs(args);

				m_StateMachine->Stop();
			}

			ClientSocket	* m_Self;
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
				m_Socket.Connect(m_HostIP, Server::PORT, 5.f);
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
				p << PT_ClientName << m_Utf8EncodedName.c_str();
				sf::Socket::Status s = m_Socket.Send(p);

				// Error checking
				if (s != sf::Socket::Done)
					std::cout << "[Client] Error sending name in Actions::SendName. Error code: " << s << std::endl;
			}

			std::string		m_Utf8EncodedName;
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

		struct SendPosition : public ActionBase
		{
			SendPosition(sf::TcpSocket &socket) : ActionBase(socket)	{ ; }

			virtual void operator()()
			{
				sf::Packet p;
				p << PT_GameContextPacket << BCPT_PlayerChoosePos << m_PosName;
				sf::Socket::Status s = m_Socket.Send(p);

				// Error checking
				if (s != sf::Socket::Done)
					std::cout << "[Client] Error sending position in Actions::SendPosition. Error code: " << s << std::endl;
			}

			std::string		m_PosName;
		};

		struct StartGame : public ActionBase
		{
			StartGame(sf::TcpSocket &socket) : ActionBase(socket)	{ ; }
			virtual void operator()()
			{
				sf::Packet p;
				p << PT_GameContextPacket << BCPT_StartGameRequest;
				m_Socket.Send(p);
			}
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
	ClientSocketPrivate(ClientSocket *self)
		: m_Thread(0), m_Self(self)
	{
		m_Thread = new sf::Thread(&ClientSocketPrivate::ThreadEP, this);

		// State machine definition
		m_StateMachine = new StateMachine();
		
		// States
		m_StateWfc			= new States::WaitingForConnection(m_StateMachine);
		m_StateConnecting	= new States::Connecting(m_StateMachine, m_Socket);
		m_StateLobbyFull	= new States::LobbyFull(m_StateMachine, m_Self);
		m_StateConnected	= new States::Connected(m_StateMachine, m_Self);
		m_StateIdle			= new States::Idle(m_StateMachine, m_Socket, m_Self);
		m_StateDisconnected	= new States::Disconnected(m_StateMachine, m_Self);

		// Actions
		m_ActionConnect		= new Actions::Connect(m_Socket);
		m_ActionSendName	= new Actions::SendName(m_Socket);
		m_ActionSendTxtMsg	= new Actions::SendTextMessage(m_Socket);
		m_ActionSendPos		= new Actions::SendPosition(m_Socket);
		m_ActionStartGame	= new Actions::StartGame(m_Socket);
		m_ActionDisconnect	= new Actions::Disconnect(m_Socket);
		
		// Transitions
		m_StateWfc			->AddTransition(NEC_ConnectionRequest,		m_StateConnecting,		m_ActionConnect		);
		m_StateConnecting	->AddTransition(NEC_Connected,				m_StateConnected							);
		m_StateConnecting	->AddTransition(NEC_CantConnect,			m_StateDisconnected							);
		m_StateConnecting	->AddTransition(NEC_LobbyFull,				m_StateLobbyFull							);
		m_StateLobbyFull	->AddTransition(NEC_DisconnectionRequest,	m_StateDisconnected,	m_ActionDisconnect	);
		m_StateConnected	->AddTransition(NEC_SendName,				m_StateIdle,			m_ActionSendName	);
		m_StateIdle			->AddTransition(NEC_SendTextMessage,		m_StateIdle,			m_ActionSendTxtMsg	);
		m_StateIdle			->AddTransition(NEC_SendPlayerPosition,		m_StateIdle,			m_ActionSendPos		);
		m_StateIdle			->AddTransition(NEC_StartGame,				m_StateIdle,			m_ActionStartGame	);
		m_StateIdle			->AddTransition(NEC_DisconnectionRequest,	m_StateDisconnected,	m_ActionDisconnect	);
	}

	void Connect(const std::string &hostIP, const std::string &utf8EncodedName)
	{
		m_DisconnectRequested				= false;
		m_ActionConnect->m_HostIP			= sf::IpAddress(hostIP);
		m_ActionSendName->m_Utf8EncodedName	= utf8EncodedName;

		m_Thread->Launch();
	}

	void Disconnect()
	{
		m_DisconnectRequested = true;
	}

	void SendChatMessage(const std::string &utf8EncodedMessage)
	{
		m_ActionSendTxtMsg->m_Utf8EncodedText = utf8EncodedMessage;
		m_StateMachine->Notify(NEC_SendTextMessage);
	}

	void ChoosePosition(const std::string &posName)
	{
		m_ActionSendPos->m_PosName = posName;
		m_StateMachine->Notify(NEC_SendPlayerPosition);
	}

	void StartGame()
	{
		m_StateMachine->Notify(NEC_StartGame);
	}

	void Wait()
	{
		m_Thread->Wait();
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
	}

private:
	sf::IpAddress	m_HostIP;
	sf::TcpSocket	m_Socket;
	sf::Thread		* m_Thread;
	ClientSocket	* m_Self;

	// Flags
	bool	m_DisconnectRequested;

	// State machine
	StateMachine	* m_StateMachine;

	State		* m_StateWfc,
				* m_StateConnecting,
				* m_StateConnected,
				* m_StateLobbyFull,
				* m_StateIdle,
				* m_StateDisconnected;
	
	Actions::Connect			* m_ActionConnect;
	Actions::SendName			* m_ActionSendName;
	Actions::SendTextMessage	* m_ActionSendTxtMsg;
	Actions::SendPosition		* m_ActionSendPos;
	Action						* m_ActionStartGame;
	Action						* m_ActionDisconnect;
};

ClientSocket::ClientSocket()
	: m_IsConnectionStatusReady(false), m_IsDisconnecting(false)
{
	m_priv = new ClientSocketPrivate(this);
}

ClientSocket::~ClientSocket()
{
	delete m_priv;
}

void ClientSocket::Connect(const std::string &hostIP, const std::string &utf8EncodedName)
{
	m_IsConnectionStatusReady	= false;
	m_IsDisconnecting			= false;
	m_GameStarting				= false;

	m_priv->Connect(hostIP, utf8EncodedName);
}

void ClientSocket::Disconnect()
{
	m_IsDisconnecting = true;
	m_priv->Disconnect();
}

void ClientSocket::SendChatMessage(const std::string &utf8EncodedMessage)
{
	m_priv->SendChatMessage(utf8EncodedMessage);
}

void ClientSocket::ChoosePosition(const std::string &posName)
{
	m_priv->ChoosePosition(posName);
}

void ClientSocket::StartGame()
{
	m_priv->StartGame();
}

void ClientSocket::EnqueueBroadcastedText(const TextBroadcastedEventArgs &args)
{
	sf::Lock lock(m_TextBroadcastedQueueMutex);
	m_TextBroadcastedQueue.push(args);
}

void ClientSocket::EnqueuePlayerConnected(const PlayerConnectedEventArgs &args)
{
	sf::Lock lock(m_PlayerConnectedQueueMutex);
	m_PlayerConnectedQueue.push(args);
}

void ClientSocket::SetConnectionStatusArgs(const ConnectionStatusEventArgs &args)
{
	if (m_IsConnectionStatusReady && m_ConnectionStateEventArgs.m_ConnectionStatus != ConnectionStatusEventArgs::CS_Connected)
	{
		return;
	}

	m_ConnectionStateEventArgs = args;
	m_IsConnectionStatusReady = true;
}

void ClientSocket::SetCurrentPositioningArgs(const CurrentPositioningArgs &args)
{
	m_CurrentPositioningArgs = args;
	m_AreCurrentPositioningArgsAvailable = true;
}

void ClientSocket::SetCardsInHandArgs(const CurrentCardsInHandArgs &args)
{
	m_CurrentCardsInHandArgs = args;
	m_CardsInHandReceived = true;
}

void ClientSocket::Update()
{
	if (m_IsConnectionStatusReady && !m_IsDisconnecting)
	{
		fireEvent(EventConnectionStatusUpdated, m_ConnectionStateEventArgs, EventNamespace);
		m_IsConnectionStatusReady = false;
	}

	if (m_AreCurrentPositioningArgsAvailable)
	{
		fireEvent(EventCurrentPositioningSent, m_CurrentPositioningArgs, EventNamespace);
		m_AreCurrentPositioningArgsAvailable = false;
	}

	if (m_GameStarting)
	{
		CEGUI::EventArgs nullArgs;
		fireEvent(EventGameStarting, nullArgs, EventNamespace);
		m_GameStarting = false;
	}

	if (m_CardsInHandReceived)
	{
		fireEvent(EventCardsReceived, m_CurrentCardsInHandArgs, EventNamespace);
		m_CardsInHandReceived = false;
	}

	UpdateMessageQueue(m_TextBroadcastedQueue, m_TextBroadcastedQueueMutex, EventTextBroadcasted);
	UpdateMessageQueue(m_PlayerConnectedQueue, m_PlayerConnectedQueueMutex, EventPlayerConnected);
}

void ClientSocket::Wait()
{
	m_priv->Wait();
}
