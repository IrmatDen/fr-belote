#ifndef CUTE_BELOTE_PLAYER_SOCKET_H
#define CUTE_BELOTE_PLAYER_SOCKET_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include "ClientSocket.h"

class PlayerSocket : public QObject, public ClientSocket
{
    Q_OBJECT

public:
	PlayerSocket(QObject *parent = nullptr);

	void	Update();

	virtual void	OnConnectionStatusChanged(ConnectionStatus newStatus);
	virtual void	OnPlayerConnected(const std::string &playerName);
	virtual void	OnPlayerDisconnected(const std::string &playerName);
	virtual void	OnTextBroadcasted(const std::string &sayer, const std::string &msg);
	virtual void	OnSysMsgBroadcasted(const std::string &msg);
	virtual void	OnPositionningReceived(const PositionningPacket &positionning);
	virtual void	OnGameStarting();
	virtual void	OnPlayerDealing(const std::string &dealerName);
	virtual void	OnCardsDealt(const CardsDealtPacket &cards);
	virtual void	OnPotentialAssetReceived(const std::string &assetCard);
	virtual void	OnAskingRevealedAsset();
	virtual void	OnAskingAnotherAsset();
	virtual void	OnAcceptedAsset(const AcceptedAssetPacket &acceptedAsset);
	virtual void	OnRefusedAsset(int refusingPlayerPos);
	virtual void	OnTurnStarting();
	virtual void	OnWaitingPlay(const WaitingPlayPacket &waitingPlay);
	virtual void	OnPlayedCard(const PlayedCardPacket &playedCard);
	virtual void	OnCurrentScores(int NSScore, int WEScore);
	virtual void	OnTotalScores(int NSScore, int WEScore);
	virtual void	OnBeloteAnnounced(int announcingPos);
	virtual void	OnRebeloteAnnounced(int announcingPos);
	virtual void	OnNoAssetTaken();
	virtual void	OnContractingTeamResult(bool isNSTeamContracting, bool hasWon);
	virtual void	OnLitige(int litigeValue);
	virtual void	OnMatchWon(bool isWonByNSTeam);

public slots:
    void    UnseatMe()      { ClientSocket::UnseatMe(); }
    void    StartGame()     { ClientSocket::StartGame(); }

signals:
    void    ConnectionStatusChanged(ClientSocket::ConnectionStatus newStatus, ClientSocket::ConnectionStatus prevStatus);
    void    PlayerConnectionStatusChanged(const QString &playerName, bool connected);
    void    TextBroadcasted(const QString &sayer, const QString &msg);
    void    SysMsgBroadcasted(const QString &msg);
    void    PositionningReceived(const QStringList &playersPos);
    void    GameStarting();
    void    PlayerDealing(const QString &dealerName);
    void    CardsDealt(const QStringList &cardsInHand);
    void    PotentialAssetReceived(const QString &assetCard);
    void    AskingRevealedAsset();
    void    AskingAnotherAsset();
    void    AcceptedAsset(int takerPos, const QString &asset, bool acceptedByNSTeam);
    void    RefusedAsset(int refusingPlayerPos);
    void    TurnStarting();
    void    WaitingPlay(const QStringList &allowedCards);
    void    PlayedCard(int player, const QString &card);
    void    CurrentScores(int NSScore, int WEScore);
    void    TotalScores(int NSScore, int WEScore);
    void    BeloteAnnounced(int announcingPos);
    void    RebeloteAnnounced(int announcingPos);
    void    NoAssetTaken();
    void    ContractingTeamResult(bool isNSTeamContracting, bool hasWon);
    void    Litige(int litigeValue);
    void    MatchWon(bool isWonByNSTeam);

private:
    ConnectionStatus  mCurrentConnectionStatus;
};

#endif
