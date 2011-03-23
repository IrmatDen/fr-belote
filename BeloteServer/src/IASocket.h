#ifndef BELOTESERVER_IASOCKET_H
#define BELOTESERVER_IASOCKET_H

#include <xxshared>

#include "ClientSocket.h"

class IASocket : public ClientSocket
{
public:
	IASocket();

	virtual void	OnConnectionStatusChanged(ConnectionStatus newStatus);
	virtual void	OnPositionningReceived(const PositionningPacket &positionning);
	/*virtual void	OnGameStarting();
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
	virtual void	OnMatchWon(bool isWonByNSTeam);*/

private:
	static const std::string	BotNames[];
	static int					k_CurrentBotIdx;

private:
	bool	m_Seated;
};

typedef std::shared_ptr<IASocket> IASocketPtr;

#endif
