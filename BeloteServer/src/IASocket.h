#ifndef BELOTESERVER_IASOCKET_H
#define BELOTESERVER_IASOCKET_H

#include <memory>

#include <boost/bind.hpp>

#include "Timer.h"

#include "ClientSocket.h"
#include "BeloteContextTypes.h"

class IASocket : public ClientSocket
{
public:
	IASocket();
	~IASocket();

	virtual void	OnConnectionStatusChanged(ConnectionStatus newStatus);
	virtual void	OnPositionningReceived(const PositionningPacket &positionning);
	virtual void	OnCardsDealt(const CardsDealtPacket &cards);
	virtual void	OnPotentialAssetReceived(const std::string &assetCard);
	virtual void	OnAskingRevealedAsset();
	virtual void	OnAskingAnotherAsset();
	virtual void	OnAcceptedAsset(const AcceptedAssetPacket &acceptedAsset);
	virtual void	OnWaitingPlay(const WaitingPlayPacket &waitingPlay);

protected:
	bool	botInNSTeam() const
	{
		return m_MySeat == PP_North || m_MySeat == PP_South;
	}

	template <typename Func>
	void	DelayReaction(Func f, int minDelay = 250, int maxDelay = 2000)
	{
		assert(minDelay >= 0);
		assert(minDelay < maxDelay);
		const sf::Uint32 reactionTime = static_cast<sf::Uint32>(sf::Randomizer::Random(minDelay, maxDelay));
		Timer::SingleShot(reactionTime, Timer::TimeoutFunc(f));
	}

private:
	size_t	CountCardsForAsset(char asset) const;
	bool	HasBelote(char asset) const;
	void	FinalizeAssetProposal(char asset, size_t cardsCountInHand);

protected:
	PlayerHand		m_MyHand;
	std::string		m_Asset;
	bool			m_AssetTakenByOpponent;

private:
	std::string		m_MyName;
	int				m_MyNameIndex;
	int				m_MySeat;
	int				m_PartnerSeat;
};

typedef std::shared_ptr<IASocket> IASocketPtr;

#endif
