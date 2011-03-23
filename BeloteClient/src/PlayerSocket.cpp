#include "Tools.h"

#include "PlayerSocket.h"

const CEGUI::String PlayerSocket::EventNamespace("ClientSocket");
const CEGUI::String PlayerSocket::EventConnectionStatusUpdated("ConnectionStatusUpdated");
const CEGUI::String PlayerSocket::EventPlayerConnected("PlayerConnected");
const CEGUI::String PlayerSocket::EventTextBroadcasted("TextBroadcasted");
const CEGUI::String PlayerSocket::EventSystemMessageBroadcasted("SystemMessageBroadcasted");
const CEGUI::String PlayerSocket::EventCurrentPositioningSent("CurrentPositioningSent");
const CEGUI::String PlayerSocket::EventGameStarting("GameStarting");
const CEGUI::String PlayerSocket::EventPlayerDealing("PlayerDealing");
const CEGUI::String PlayerSocket::EventCardsReceived("CardsReceived");
const CEGUI::String PlayerSocket::EventPotentialAsset("PotentialAsset");
const CEGUI::String PlayerSocket::EventAskRevealedAsset("AskRevealedAsset");
const CEGUI::String PlayerSocket::EventAskAnotherAsset("AskAnotherAsset");
const CEGUI::String PlayerSocket::EventPlayerRefusedAsset("PlayerRefusedAsset");
const CEGUI::String PlayerSocket::EventPlayerAcceptedAsset("PlayerAcceptedAsset");
const CEGUI::String PlayerSocket::EventTurnStarting("TurnStarting");
const CEGUI::String PlayerSocket::EventWaitingPlay("WaitingPlay");
const CEGUI::String PlayerSocket::EventPlayedCard("PlayedCard");
const CEGUI::String PlayerSocket::EventCurrentScores("CurrentScores");
const CEGUI::String PlayerSocket::EventTotalScores("TotalScores");
const CEGUI::String PlayerSocket::EventBeloteAnnounced("BeloteAnnounced");
const CEGUI::String PlayerSocket::EventRebeloteAnnounced("RebeloteAnnounced");
const CEGUI::String PlayerSocket::EventNoAssetTaken("NoAssetTaken");
const CEGUI::String PlayerSocket::EventContractingTeamResult("ContractingTeamResult");
const CEGUI::String PlayerSocket::EventLitige("Litige");
const CEGUI::String PlayerSocket::EventMatchWon("MatchWon");

PlayerSocket::PlayerSocket()
:	m_PlayerConnected			(EventPlayerConnected, EventNamespace),
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
	m_TotalScores				(EventTotalScores, EventNamespace),
	m_BeloteAnnounced			(EventBeloteAnnounced, EventNamespace),
	m_RebeloteAnnounced			(EventRebeloteAnnounced, EventNamespace),
	m_NoAssetTaken				(EventNoAssetTaken, EventNamespace),
	m_ContractingTeamResult		(EventContractingTeamResult, EventNamespace),
	m_Litige					(EventLitige, EventNamespace),
	m_MatchWon					(EventMatchWon, EventNamespace)
{
}

void PlayerSocket::Update()
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
	m_TotalScores.process(this);
	m_BeloteAnnounced.process(this);
	m_RebeloteAnnounced.process(this);
	m_NoAssetTaken.process(this);
	m_ContractingTeamResult.process(this);
	m_Litige.process(this);
	m_MatchWon.process(this);
}

void PlayerSocket::OnConnectionStatusChanged(ConnectionStatus newStatus)
{
	ConnectionStatusEventArgs args;
	args.m_ConnectionStatus = newStatus;
	m_ConnectionStatus.push(m_ConnectionStatusPushGuard, args);
}

void PlayerSocket::OnPlayerConnected(const std::string &playerName)
{
	PlayerConnectedEventArgs args;
	args.m_PlayerName = playerName;
	args.m_Connected = true;
	m_PlayerConnected.push(args);
}

void PlayerSocket::OnPlayerDisconnected(const std::string &playerName)
{
	PlayerConnectedEventArgs args;
	args.m_PlayerName = playerName;
	args.m_Connected = false;
	m_PlayerConnected.push(args);
}

void PlayerSocket::OnTextBroadcasted(const std::string &sayer, const std::string &msg)
{
	TextBroadcastedEventArgs args;
	args.m_Teller = sayer;
	args.m_Message = msg;
	m_TextBroadcasted.push(args);
}

void PlayerSocket::OnSysMsgBroadcasted(const std::string &msg)
{
	SystemMessageBroadcastedEventArgs args;
	args.m_Message = msg;
	m_SystemMessageBroadcasted.push(args);
}

void PlayerSocket::OnPositionningReceived(const PositionningPacket &positionning)
{
	CurrentPositioningArgs args;
	for (sf::Uint32 i = 0; i != countof(args.m_Pos); i++)
		args.m_Pos[i] = positionning.m_Names[i];
	m_CurrentPositioningSent.push(args);
}

void PlayerSocket::OnGameStarting()
{
	m_GameStarting.push();
}

void PlayerSocket::OnPlayerDealing(const std::string &dealerName)
{
	PlayerDealingArgs args;
	args.m_Who = dealerName;
	m_PlayerDealing.push(args);
}

void PlayerSocket::OnCardsDealt(const CardsDealtPacket &cards)
{
	CurrentCardsInHandArgs args;
	for (sf::Uint32 i = 0; i != countof(args.m_Cards); i++)
		args.m_Cards[i] = cards.m_Cards[i];
	m_CardsReceived.push(args);
}

void PlayerSocket::OnPotentialAssetReceived(const std::string &assetCard)
{
	PotentialAssetArgs args;
	args.m_Card = assetCard;
	m_PotentialAsset.push(args);
}

void PlayerSocket::OnAskingRevealedAsset()
{
	m_AskRevealedAsset.push();
}

void PlayerSocket::OnAskingAnotherAsset()
{
	m_AskAnotherAsset.push();
}

void PlayerSocket::OnAcceptedAsset(const AcceptedAssetPacket &acceptedAsset)
{
	PlayerAcceptedAssetArgs args;
	args.m_PlayerPos = acceptedAsset.m_PlayerPos;
	args.m_Asset = acceptedAsset.m_Asset;
	args.m_AcceptedByNSTeam = acceptedAsset.m_AcceptedByNSTeam;
	m_PlayerAcceptedAsset.push(args);
}

void PlayerSocket::OnRefusedAsset(int refusingPlayerPos)
{
	PlayerRefusedAssetArgs args;
	args.m_PlayerPos = refusingPlayerPos;
	m_PlayerRefusedAsset.push(args);
}

void PlayerSocket::OnTurnStarting()
{
	m_TurnStarting.push();
}

void PlayerSocket::OnWaitingPlay(const WaitingPlayPacket &waitingPlay)
{
	WaitingPlayArgs args;
	for (sf::Uint32 i = 0; i != waitingPlay.m_PossibleCardsCount; i++)
		args.m_PossibleCards[i] = waitingPlay.m_PossibleCards[i];
	m_WaitingPlay.push(args);
}

void PlayerSocket::OnPlayedCard(const PlayedCardPacket &playedCard)
{
	PlayedCardArgs args;
	args.m_Card = playedCard.m_Card;
	args.m_Player = playedCard.m_Player;
	m_PlayedCard.push(args);
}

void PlayerSocket::OnCurrentScores(int NSScore, int WEScore)
{
	ScoresArgs args;
	args.m_NorthSouthScore = NSScore;
	args.m_WestEastScore = WEScore;
	m_CurrentScores.push(args);
}

void PlayerSocket::OnTotalScores(int NSScore, int WEScore)
{
	ScoresArgs args;
	args.m_NorthSouthScore = NSScore;
	args.m_WestEastScore = WEScore;
	m_TotalScores.push(args);
}

void PlayerSocket::OnBeloteAnnounced(int announcingPos)
{
	BeloteAnnouncedArgs args;
	args.m_ByPlayerPos = announcingPos;
	m_BeloteAnnounced.push(args);
}

void PlayerSocket::OnRebeloteAnnounced(int announcingPos)
{
	BeloteAnnouncedArgs args;
	args.m_ByPlayerPos = announcingPos;
	m_RebeloteAnnounced.push(args);
}

void PlayerSocket::OnNoAssetTaken()
{
	m_NoAssetTaken.push();
}

void PlayerSocket::OnContractingTeamResult(bool isNSTeamContracting, bool hasWon)
{
	ContractingTeamResultArgs args;
	args.m_IsNorthSouthTeam = isNSTeamContracting;
	args.m_HasWon = hasWon;
	m_ContractingTeamResult.push(args);
}

void PlayerSocket::OnLitige(int litigeValue)
{
	LitigeArgs args;
	args.m_LitigeValue = litigeValue;
	m_Litige.push(args);
}

void PlayerSocket::OnMatchWon(bool isWonByNSTeam)
{
	MatchWonArgs args;
	args.m_MatchWonByNSTeam = isWonByNSTeam;
	m_MatchWon.push(args);
}
