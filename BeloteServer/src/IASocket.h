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

	virtual void	OnTurnStarting();
	virtual void	OnPlayedCard(const PlayedCardPacket &playedCard);
	virtual void	OnWaitingPlay(const WaitingPlayPacket &waitingPlay);

private:
	bool	botInNSTeam() const
	{
		return m_MySeat == PP_North || m_MySeat == PP_South;
	}

	size_t	CountCardsForColour(char col) const;
	bool	HasBelote(char asset) const;
	void	FinalizeAssetProposal(char asset, size_t cardsCountInHand);
	
	bool		IsFirstPlayingInTurn() const;
	bool		IsPartnerOwningTurn() const;
	std::string	GetBestOpener() const;
	bool		ShouldPlayAsset() const;
	std::string	GetStrongestCardForColour(const std::string &colour, const PlayerHand &cardsToCheck) const;
	std::string	GetWeakestCardForColour(const std::string &colour, const PlayerHand &cardsToCheck) const;
	bool		IsCardOwningTurn(const std::string & card) const;

	template <typename Func>
	void	DelayReaction(Func f, int minDelay = 250, int maxDelay = 2000)
	{
		assert(minDelay >= 0);
		assert(minDelay < maxDelay);

		const sf::Uint32 reactionTime = static_cast<sf::Uint32>(sf::Randomizer::Random(minDelay, maxDelay));
		m_Timer.SetPeriod(reactionTime);
		m_Timer.SetTimeoutFunc(f);
		m_Timer.Start();
	}

private:
	typedef boost::array<int, 32> DeckPlayed;

private:
	Timer			m_Timer;

	std::string		m_MyName;
	int				m_MyNameIndex;
	int				m_MySeat;
	int				m_PartnerSeat;

	PlayerHand		m_MyHand;
	std::string		m_Asset;
	bool			m_AssetTakenByOpponent;

	int				m_CardsRemainingInHand;
	int				m_AssetsPlayed;
	std::string		m_FirstCardThisTurn;

	boost::array<std::string, 4>	m_CurrentTurnCards;
	DeckPlayed						m_PlayedCards;
};

typedef std::shared_ptr<IASocket> IASocketPtr;

#endif
