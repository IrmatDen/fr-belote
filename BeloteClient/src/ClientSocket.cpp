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
const CEGUI::String ClientSocket::EventTextBroadcasted("TextBroadcasted");
const CEGUI::String ClientSocket::EventSystemMessageBroadcasted("SystemMessageBroadcasted");
const CEGUI::String ClientSocket::EventCurrentPositioningSent("CurrentPositioningSent");
const CEGUI::String ClientSocket::EventGameStarting("GameStarting");
const CEGUI::String ClientSocket::EventPlayerDealing("PlayerDealing");
const CEGUI::String ClientSocket::EventCardsReceived("CardsReceived");
const CEGUI::String ClientSocket::EventPotentialAsset("PotentialAsset");
const CEGUI::String ClientSocket::EventAskRevealedAsset("AskRevealedAsset");
const CEGUI::String ClientSocket::EventAskAnotherAsset("AskAnotherAsset");
const CEGUI::String ClientSocket::EventPlayerRefusedAsset("PlayerRefusedAsset");
const CEGUI::String ClientSocket::EventPlayerAcceptedAsset("PlayerAcceptedAsset");
const CEGUI::String ClientSocket::EventTurnStarting("TurnStarting");
const CEGUI::String ClientSocket::EventWaitingPlay("WaitingPlay");
const CEGUI::String ClientSocket::EventPlayedCard("PlayedCard");
const CEGUI::String ClientSocket::EventCurrentScores("CurrentScores");
const CEGUI::String ClientSocket::EventBeloteAnnounced("BeloteAnnounced");

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
		NEC_AcceptAsset,
		NEC_RefuseAsset,
		NEC_DisconnectionRequest,
		NEC_PlayCard,
	};

	namespace States
	{
		struct WaitingForConnection : public State
		{
			WaitingForConnection(StateMachinePtr sm) : State(sm)	{ ; }

			virtual void	Enter()		{ m_StateMachine->Notify(NEC_ConnectionRequest); }
		};
	
		struct Connecting : public State
		{
			Connecting(StateMachinePtr sm, sf::TcpSocket &socket)
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
			Connected(StateMachinePtr sm, ClientSocket *self)
				: State(sm), m_Self(self)
			{ ; }

			virtual void	Enter()
			{
				ConnectionStatusEventArgs args;
				args.m_ConnectionStatus = ConnectionStatusEventArgs::CS_Connected;
				
				m_Self->m_ConnectionStatus.push(m_Self->m_ConnectionStatusPushGuard, args);
				m_StateMachine->Notify(NEC_SendName);
			}

			ClientSocket	* m_Self;
		};

		struct Idle : public State
		{
			Idle(StateMachinePtr sm, sf::TcpSocket &socket, ClientSocket *self)
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
							m_Self->m_PlayerConnected.push(args);
						}
						break;
						
					case PT_ClientDisconnected:
						{
							PlayerConnectedEventArgs args;
							packet >> args.m_PlayerName;
							args.m_Connected = false;
							m_Self->m_PlayerConnected.push(args);
						}
						break;

					case PT_ServerBroadcastTextMessage:
						{
							TextBroadcastedEventArgs args;
							packet >> args.m_Teller >> args.m_Message;
							m_Self->m_TextBroadcasted.push(args);
						}
						break;

					case PT_ServerBroadcastSystemMessage:
						{
							SystemMessageBroadcastedEventArgs args;
							packet >> args.m_Message;
							m_Self->m_SystemMessageBroadcasted.push(args);
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
						m_Self->m_CurrentPositioningSent.push(args);
					}
					break;
					
				case BCPT_GameStarting:
					m_Self->m_GameStarting.push();
					break;

				case BCPT_Dealing:
					{
						PlayerDealingArgs args;
						packet >> args.m_Who;
						m_Self->m_PlayerDealing.push(args);
					}
					break;

				case BCPT_CardsDealt:
					{
						CurrentCardsInHandArgs args;
						for (sf::Uint32 i = 0; i != countof(args.m_Cards); i++)
							packet >> args.m_Cards[i];
						m_Self->m_CardsReceived.push(args);
					}
					break;

				case BCPT_PotentialAsset:
					{
						PotentialAssetArgs args;
						packet >> args.m_Card;
						m_Self->m_PotentialAsset.push(args);
					}
					break;

				case BCPT_AskRevealedAsset:
					m_Self->m_AskRevealedAsset.push();
					break;

				case BCPT_AskAnotherAsset:
					m_Self->m_AskAnotherAsset.push();
					break;

				case BCPT_AssetAccepted:
					{
						PlayerAcceptedAssetArgs args;
						packet >> args.m_ByPlayer >> args.m_Asset;
						m_Self->m_PlayerAcceptedAsset.push(args);
					}
					break;

				case BCPT_AssetRefused:
					{
						PlayerRefusedAssetArgs args;
						packet >> args.m_ByPlayer;
						m_Self->m_PlayerRefusedAsset.push(args);
					}
					break;

				case BCPT_TurnStarting:
					m_Self->m_TurnStarting.push();
					break;

				case BCPT_WaitingPlay:
					{
						WaitingPlayArgs args;
						int possibleCardsCount;
						packet >> possibleCardsCount;
						for (sf::Uint32 i = 0; i != possibleCardsCount; i++)
							packet >> args.m_PossibleCards[i];
						m_Self->m_WaitingPlay.push(args);
					}
					break;

				case BCPT_CardPlayed:
					{
						PlayedCardArgs args;
						packet >> args.m_Card >> args.m_Player;
						m_Self->m_PlayedCard.push(args);
					}
					break;

				case BCPT_CurrentScores:
					{
						CurrentScoresArgs args;
						packet >> args.m_NorthSouthScore >> args.m_WestEastScore >> args.m_PlayedLastTurn;
						m_Self->m_CurrentScores.push(args);
					}
					break;

				case BCPT_BeloteAnnounced:
					{
						BeloteAnnouncedArgs args;
						packet >> args.m_ByPlayer;
						m_Self->m_BeloteAnnounced.push(args);
					}
					break;
				}
			}
		};

		struct LobbyFull : public State
		{
			LobbyFull(StateMachinePtr sm, ClientSocket *self)
				: State(sm), m_Self(self)
			{ ; }

			virtual void	Enter()
			{
				ConnectionStatusEventArgs args;
				args.m_ConnectionStatus = ConnectionStatusEventArgs::CS_LobbyFull;
				m_Self->m_ConnectionStatus.push(m_Self->m_ConnectionStatusPushGuard, args);

				m_StateMachine->Notify(NEC_DisconnectionRequest);
			}

			ClientSocket	* m_Self;
		};

		struct Disconnected : public State
		{
			Disconnected(StateMachinePtr sm, ClientSocket *self)
				: State(sm), m_Self(self)
			{ ; }

			virtual void	Enter()
			{
				ConnectionStatusEventArgs args;
				args.m_ConnectionStatus = ConnectionStatusEventArgs::CS_Disconnected;
				m_Self->m_ConnectionStatus.push(m_Self->m_ConnectionStatusPushGuard, args);

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
		typedef std::shared_ptr<ActionBase>	ActionBasePtr;

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
		typedef std::shared_ptr<Connect>	ConnectPtr;

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
		typedef std::shared_ptr<SendName>	SendNamePtr;

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
		typedef std::shared_ptr<SendTextMessage>	SendTextMessagePtr;

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
		typedef std::shared_ptr<SendPosition>	SendPositionPtr;

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

		struct AcceptAsset : public ActionBase
		{
			AcceptAsset(sf::TcpSocket &socket) : ActionBase(socket)	{ ; }

			virtual void operator()()
			{
				sf::Packet p;
				p << PT_GameContextPacket << BCPT_AcceptAsset << m_AssetColour;
				sf::Socket::Status s = m_Socket.Send(p);

				// Error checking
				if (s != sf::Socket::Done)
					std::cout << "[Client] Error sending position in Actions::AcceptAsset. Error code: " << s << std::endl;
			}

			std::string		m_AssetColour;
		};
		typedef std::shared_ptr<AcceptAsset>	AcceptAssetPtr;

		struct RefuseAsset : public ActionBase
		{
			RefuseAsset(sf::TcpSocket &socket) : ActionBase(socket)	{ ; }
			virtual void operator()()
			{
				sf::Packet p;
				p << PT_GameContextPacket << BCPT_RefuseAsset;
				m_Socket.Send(p);
			}
		};

		struct PlayCard : public ActionBase
		{
			PlayCard(sf::TcpSocket &socket) : ActionBase(socket)	{ ; }

			virtual void operator()()
			{
				sf::Packet p;
				p << PT_GameContextPacket << BCPT_PlayCard << m_CardName;
				sf::Socket::Status s = m_Socket.Send(p);

				// Error checking
				if (s != sf::Socket::Done)
					std::cout << "[Client] Error sending card in Actions::PlayCard. Error code: " << s << std::endl;
			}

			std::string		m_CardName;
		};
		typedef std::shared_ptr<PlayCard>	PlayCardPtr;

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
		m_StateMachine = StateMachinePtr(new StateMachine());
		
		// States
		m_StateWfc			= StatePtr(new States::WaitingForConnection(m_StateMachine));
		m_StateConnecting	= StatePtr(new States::Connecting(m_StateMachine, m_Socket));
		m_StateLobbyFull	= StatePtr(new States::LobbyFull(m_StateMachine, m_Self));
		m_StateConnected	= StatePtr(new States::Connected(m_StateMachine, m_Self));
		m_StateIdle			= StatePtr(new States::Idle(m_StateMachine, m_Socket, m_Self));
		m_StateDisconnected	= StatePtr(new States::Disconnected(m_StateMachine, m_Self));

		// Actions
		m_ActionConnect		= Actions::ConnectPtr			(new Actions::Connect(m_Socket));
		m_ActionSendName	= Actions::SendNamePtr			(new Actions::SendName(m_Socket));
		m_ActionSendTxtMsg	= Actions::SendTextMessagePtr	(new Actions::SendTextMessage(m_Socket));
		m_ActionSendPos		= Actions::SendPositionPtr		(new Actions::SendPosition(m_Socket));
		m_ActionStartGame	= ActionPtr						(new Actions::StartGame(m_Socket));
		m_ActionAcceptAsset	= Actions::AcceptAssetPtr		(new Actions::AcceptAsset(m_Socket));
		m_ActionRefuseAsset	= ActionPtr						(new Actions::RefuseAsset(m_Socket));
		m_ActionPlayCard	= Actions::PlayCardPtr			(new Actions::PlayCard(m_Socket));
		m_ActionDisconnect	= ActionPtr						(new Actions::Disconnect(m_Socket));
		
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
		m_StateIdle			->AddTransition(NEC_AcceptAsset,			m_StateIdle,			m_ActionAcceptAsset	);
		m_StateIdle			->AddTransition(NEC_RefuseAsset,			m_StateIdle,			m_ActionRefuseAsset	);
		m_StateIdle			->AddTransition(NEC_PlayCard,				m_StateIdle,			m_ActionPlayCard	);
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

	void AcceptAsset(const std::string &assetColour)
	{
		m_ActionAcceptAsset->m_AssetColour = assetColour;
		m_StateMachine->Notify(NEC_AcceptAsset);
	}

	void RefuseAsset()
	{
		m_StateMachine->Notify(NEC_RefuseAsset);
	}

	void PlayCard(const std::string &cardName)
	{
		m_ActionPlayCard->m_CardName = cardName;
		m_StateMachine->Notify(NEC_PlayCard);
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
	StateMachinePtr	m_StateMachine;

	StatePtr	m_StateWfc,
				m_StateConnecting,
				m_StateConnected,
				m_StateLobbyFull,
				m_StateIdle,
				m_StateDisconnected;
	
	Actions::ConnectPtr			m_ActionConnect;
	Actions::SendNamePtr		m_ActionSendName;
	Actions::SendTextMessagePtr	m_ActionSendTxtMsg;
	Actions::SendPositionPtr	m_ActionSendPos;
	ActionPtr					m_ActionStartGame;
	Actions::AcceptAssetPtr		m_ActionAcceptAsset;
	ActionPtr					m_ActionRefuseAsset;
	ActionPtr					m_ActionDisconnect;
	Actions::PlayCardPtr		m_ActionPlayCard;
};

ClientSocket::ClientSocket()
: m_IsDisconnecting(false),
	m_PlayerConnected			(EventPlayerConnected, EventNamespace),
	m_TextBroadcasted			(EventTextBroadcasted, EventNamespace),
	m_SystemMessageBroadcasted	(EventSystemMessageBroadcasted, EventNamespace),
	m_CardsReceived				(EventCardsReceived, EventNamespace),
	m_CurrentPositioningSent	(EventCurrentPositioningSent, EventNamespace),
	m_GameStarting				(EventGameStarting, EventNamespace),
	m_PlayerDealing				(EventPlayerDealing, EventNamespace),
	m_ConnectionStatus			(EventConnectionStatusUpdated, EventNamespace),
	m_PotentialAsset			(EventPotentialAsset, EventNamespace),
	m_AskRevealedAsset			(EventAskRevealedAsset, EventNamespace),
	m_AskAnotherAsset			(EventAskAnotherAsset, EventNamespace),
	m_PlayerRefusedAsset		(EventPlayerRefusedAsset, EventNamespace),
	m_PlayerAcceptedAsset		(EventPlayerAcceptedAsset, EventNamespace),
	m_TurnStarting				(EventTurnStarting, EventNamespace),
	m_WaitingPlay				(EventWaitingPlay, EventNamespace),
	m_PlayedCard				(EventPlayedCard, EventNamespace),
	m_CurrentScores				(EventCurrentScores, EventNamespace),
	m_BeloteAnnounced			(EventBeloteAnnounced, EventNamespace)
{
	m_priv = new ClientSocketPrivate(this);
}

ClientSocket::~ClientSocket()
{
	delete m_priv;
}

void ClientSocket::Connect(const std::string &hostIP, const std::string &utf8EncodedName)
{
	m_IsDisconnecting = false;

	m_PlayerConnected.reset();
	m_TextBroadcasted.reset();
	m_CardsReceived.reset();
	m_CurrentPositioningSent.reset();
	m_GameStarting.reset();
	m_ConnectionStatus.reset();

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

void ClientSocket::AcceptAsset(const std::string &assetColour)
{
	m_priv->AcceptAsset(assetColour);
}

void ClientSocket::RefuseAsset()
{
	m_priv->RefuseAsset();
}

void ClientSocket::PlayCard(const std::string &cardName)
{
	m_priv->PlayCard(cardName);
}

void ClientSocket::Update()
{
	// Brute force ftw!
	// Ok, I may consider storing current game phase to avoid this...

	if (!m_IsDisconnecting)
		m_ConnectionStatus.process(this);

	m_GameStarting.process(this);
	m_PlayerDealing.process(this);
	m_PlayerConnected.process(this);
	m_TextBroadcasted.process(this);
	m_SystemMessageBroadcasted.process(this);
	m_CurrentPositioningSent.process(this);
	m_CardsReceived.process(this);
	m_PotentialAsset.process(this);
	m_AskRevealedAsset.process(this);
	m_AskAnotherAsset.process(this);
	m_PlayerRefusedAsset.process(this);
	m_PlayerAcceptedAsset.process(this);
	m_TurnStarting.process(this);
	m_WaitingPlay.process(this);
	m_PlayedCard.process(this);
	m_CurrentScores.process(this);
	m_BeloteAnnounced.process(this);
}

void ClientSocket::Wait()
{
	m_priv->Wait();
}
