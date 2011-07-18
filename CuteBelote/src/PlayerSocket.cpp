#include "Tools.h"

#include "PlayerSocket.h"

PlayerSocket::PlayerSocket(QObject *parent)
    : QObject(parent), mCurrentConnectionStatus(CS_Disconnected)
{
}

void PlayerSocket::Update()
{
	ClientSocket::Update();
}

void PlayerSocket::OnConnectionStatusChanged(ConnectionStatus newStatus)
{
	emit ConnectionStatusChanged(newStatus, mCurrentConnectionStatus);
    mCurrentConnectionStatus = newStatus;
}

void PlayerSocket::OnPlayerConnected(const std::string &playerName)
{
    emit PlayerConnectionStatusChanged(QString::fromUtf8(playerName.c_str()), true);
}

void PlayerSocket::OnPlayerDisconnected(const std::string &playerName)
{
	emit PlayerConnectionStatusChanged(QString::fromUtf8(playerName.c_str()), false);
}

void PlayerSocket::OnTextBroadcasted(const std::string &sayer, const std::string &msg)
{
	emit TextBroadcasted(QString::fromUtf8(sayer.c_str()), QString::fromUtf8(msg.c_str()));
}

void PlayerSocket::OnSysMsgBroadcasted(const std::string &msg)
{
	emit SysMsgBroadcasted(QString::fromUtf8(msg.c_str()));
}

void PlayerSocket::OnPositionningReceived(const PositionningPacket &positionning)
{
	QStringList pos;
	for (int i = 0; i != 4; i++)
		pos << QString::fromStdString(positionning.m_Names[i]);
	emit PositionningReceived(pos);
}

void PlayerSocket::OnGameStarting()
{
	emit GameStarting();
}

void PlayerSocket::OnPlayerDealing(const std::string &dealerName)
{
    emit PlayerDealing(QString::fromStdString(dealerName));
}

void PlayerSocket::OnCardsDealt(const CardsDealtPacket &cards)
{
    QStringList cardsInHand;
	for (int i = 0; i != 8; i++)
		cardsInHand << QString::fromStdString(cards.m_Cards[i]);
	emit CardsDealt(cardsInHand);
}

void PlayerSocket::OnPotentialAssetReceived(const std::string &assetCard)
{
    emit PotentialAssetReceived(QString::fromStdString(assetCard));
}

void PlayerSocket::OnAskingRevealedAsset()
{
	emit AskingRevealedAsset();
}

void PlayerSocket::OnAskingAnotherAsset()
{
	emit AskingAnotherAsset();
}

void PlayerSocket::OnAcceptedAsset(const AcceptedAssetPacket &acceptedAsset)
{
    emit AcceptedAsset(acceptedAsset.m_PlayerPos,
                       QString::fromStdString(acceptedAsset.m_Asset),
                       acceptedAsset.m_AcceptedByNSTeam);
}

void PlayerSocket::OnRefusedAsset(int refusingPlayerPos)
{
	emit RefusedAsset(refusingPlayerPos);
}

void PlayerSocket::OnTurnStarting()
{
	emit TurnStarting();
}

void PlayerSocket::OnWaitingPlay(const WaitingPlayPacket &waitingPlay)
{
    QStringList allowedCards;
	for (sf::Uint32 i = 0; i != waitingPlay.m_PossibleCardsCount; i++)
		allowedCards << QString::fromStdString(waitingPlay.m_PossibleCards[i]);
	emit WaitingPlay(allowedCards);
}

void PlayerSocket::OnPlayedCard(const PlayedCardPacket &playedCard)
{
    emit PlayedCard(playedCard.m_Player, QString::fromStdString(playedCard.m_Card));
}

void PlayerSocket::OnCurrentScores(int NSScore, int WEScore)
{
    emit CurrentScores(NSScore, WEScore);
}

void PlayerSocket::OnTotalScores(int NSScore, int WEScore)
{
    emit TotalScores(NSScore, WEScore);
}

void PlayerSocket::OnBeloteAnnounced(int announcingPos)
{
    emit BeloteAnnounced(announcingPos);
}

void PlayerSocket::OnRebeloteAnnounced(int announcingPos)
{
    emit RebeloteAnnounced(announcingPos);
}

void PlayerSocket::OnNoAssetTaken()
{
	emit NoAssetTaken();
}

void PlayerSocket::OnContractingTeamResult(bool isNSTeamContracting, bool hasWon)
{
	emit ContractingTeamResult(isNSTeamContracting, hasWon);
}

void PlayerSocket::OnLitige(int litigeValue)
{
    emit Litige(litigeValue);
}

void PlayerSocket::OnMatchWon(bool isWonByNSTeam)
{
    emit MatchWon(isWonByNSTeam);
}
