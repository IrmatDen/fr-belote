#ifndef BELOTESERVER_IASOCKET_H
#define BELOTESERVER_IASOCKET_H

#include <xxshared>

#include "ClientSocket.h"
#include "BeloteContextTypes.h"

class IASocket : public ClientSocket
{
public:
	IASocket();

	virtual void	OnConnectionStatusChanged(ConnectionStatus newStatus);
	virtual void	OnPositionningReceived(const PositionningPacket &positionning);
	virtual void	OnCardsDealt(const CardsDealtPacket &cards);
	virtual void	OnPotentialAssetReceived(const std::string &assetCard);
	virtual void	OnAskingRevealedAsset();
	virtual void	OnAskingAnotherAsset();
	virtual void	OnAcceptedAsset(const AcceptedAssetPacket &acceptedAsset);
	virtual void	OnWaitingPlay(const WaitingPlayPacket &waitingPlay);

protected:
	bool	botInNSTeam() const	{ return m_Seated == 1 || m_Seated == 3; }

protected:
	PlayerHand		m_MyHand;
	std::string		m_Asset;
	bool			m_AssetTakenByOpponent;

private:
	static const std::string	BotNames[];
	static int					k_CurrentBotIdx;

private:
	int		m_Seated;
};

typedef std::shared_ptr<IASocket> IASocketPtr;

#endif
