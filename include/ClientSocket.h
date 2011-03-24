#ifndef BELOTE_CLIENTSOCKET_H
#define BELOTE_CLIENTSOCKET_H

#include <string>
#include <queue>

#include <SFML/System.hpp>

#include "BeloteContextPackets.h"

class ClientSocketPrivate;
typedef std::shared_ptr<ClientSocketPrivate> ClientSocketPrivatePtr;

class ClientSocket
{
public:
	enum ConnectionStatus
	{
		CS_Connected,
		CS_Disconnected,
		CS_LobbyFull
	};

public:
	virtual ~ClientSocket();

	void	Connect(const std::string &hostIP, const std::string &utf8EncodedName);
	void	SendChatMessage(const std::string &utf8EncodedMessage);
	void	Disconnect();

	void	ChoosePosition(const std::string &posName);
	void	UnseatMe();
	void	StartGame();
	void	AcceptAsset(const std::string &assetColour);
	void	RefuseAsset();
	void	PlayCard(const std::string &cardName);

	virtual void	Update();
	virtual void	OnConnectionStatusChanged(ConnectionStatus newStatus)					{ (void)newStatus; }
	virtual void	OnPlayerConnected(const std::string &playerName)						{ (void)playerName; }
	virtual void	OnPlayerDisconnected(const std::string &playerName)						{ (void)playerName; }
	virtual void	OnTextBroadcasted(const std::string &sayer, const std::string &msg)		{ (void)sayer; (void)msg; }
	virtual void	OnSysMsgBroadcasted(const std::string &msg)								{ (void)msg; }
	virtual void	OnPositionningReceived(const PositionningPacket &positionning)			{ (void)positionning; }
	virtual void	OnGameStarting()														{ ; }
	virtual void	OnPlayerDealing(const std::string &dealerName)							{ (void)dealerName; }
	virtual void	OnCardsDealt(const CardsDealtPacket &cards)								{ (void)cards; }
	virtual void	OnPotentialAssetReceived(const std::string &assetCard)					{ (void)assetCard; }
	virtual void	OnAskingRevealedAsset()													{ ; }
	virtual void	OnAskingAnotherAsset()													{ ; }
	virtual void	OnAcceptedAsset(const AcceptedAssetPacket &acceptedAsset)				{ (void)acceptedAsset; }
	virtual void	OnRefusedAsset(int refusingPlayerPos)									{ (void)refusingPlayerPos; }
	virtual void	OnTurnStarting()														{ ; }
	virtual void	OnWaitingPlay(const WaitingPlayPacket &waitingPlay)						{ (void)waitingPlay; }
	virtual void	OnPlayedCard(const PlayedCardPacket &playedCard)						{ (void)playedCard; }
	virtual void	OnCurrentScores(int NSScore, int WEScore)								{ (void)NSScore; (void)WEScore; }
	virtual void	OnTotalScores(int NSScore, int WEScore)									{ (void)NSScore; (void)WEScore; }
	virtual void	OnBeloteAnnounced(int announcingPos)									{ (void)announcingPos; }
	virtual void	OnRebeloteAnnounced(int announcingPos)									{ (void)announcingPos; }
	virtual void	OnNoAssetTaken()														{ ; }
	virtual void	OnContractingTeamResult(bool isNSTeamContracting, bool hasWon)			{ (void)isNSTeamContracting; (void)hasWon; }
	virtual void	OnLitige(int litigeValue)												{ (void)litigeValue; }
	virtual void	OnMatchWon(bool isWonByNSTeam)											{ (void)isWonByNSTeam; }

protected:
	ClientSocket();

	bool	m_IsDisconnecting;

private:
	ClientSocketPrivatePtr	m_priv;
};

#endif
