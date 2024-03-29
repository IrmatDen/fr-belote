#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <utility>

#include "Timer.h"
#include "Tools.h"

#include "IASocket.h"
#include "BeloteContext.h"

using namespace std;
using namespace boost;

namespace
{
	class BotNameProvider
	{
	public:
		typedef std::pair<int, std::string> NameInfo;

	public:
		static NameInfo GetName()
		{
			Init();

			const int nameIdx = FreeNames.front();
			FreeNames.pop();

			return std::make_pair(nameIdx, BotNames[nameIdx]);
		}

		static void FreeName(int nameIdx)
		{
			FreeNames.push(nameIdx);
		}

	private:
		static void Init()
		{
			if (BotNameProviderInitialized)
				return;

			for (int i = 0; i != BotNamesCount; i++)
				FreeNames.push(i);

			BotNameProviderInitialized = true;
		}

	private:
		static const int			BotNamesCount = 4;
		static const std::string	BotNames[BotNamesCount];
		static bool					BotNameProviderInitialized;
		static std::queue<int>		FreeNames;
	};
	
	const std::string	BotNameProvider::BotNames[BotNamesCount]	= { "Escartefigue", "Panisse", "César", "M. Brun" };
	bool				BotNameProvider::BotNameProviderInitialized = false;
	std::queue<int>		BotNameProvider::FreeNames;
}

IASocket::IASocket()
	: m_MySeat(-1)
{
	BotNameProvider::NameInfo ni = BotNameProvider::GetName();
	m_MyNameIndex	= ni.first;
	m_MyName		= ni.second;

	m_Timer.SetMode(Timer::TM_ONESHOT);

	Connect("127.0.0.1", m_MyName);
}

IASocket::~IASocket()
{
	BotNameProvider::FreeName(m_MyNameIndex);
}

void IASocket::OnConnectionStatusChanged(ConnectionStatus newStatus)
{
	if (newStatus == CS_Connected)
	{
		SendChatMessage("Salut");
	}
}

void IASocket::OnPositionningReceived(const PositionningPacket &positionning)
{
	// Check that the seat hasn't been stolen; if it was, just take another one.
	if (m_MySeat >= 0 && positionning.m_Names[m_MySeat] == m_MyName)
		return;

	// Fill the first free spot
	boost::array<std::string, 4>::const_iterator it = std::find(positionning.m_Names.begin(),
		positionning.m_Names.end(),
		std::string());

	if (it == positionning.m_Names.end())
	{
		std::cout << "[IASocket::OnPositionningReceived] No more free seats!" << std::endl;
		Disconnect();
	}

	m_MySeat = std::distance(positionning.m_Names.begin(), it);
	ChoosePosition(BeloteContext::PlayerPositionStrings[m_MySeat]);

	m_PartnerSeat = m_MySeat + 2;
	if (m_PartnerSeat >= 4)
		m_PartnerSeat = m_PartnerSeat - 4;
}

void IASocket::OnCardsDealt(const CardsDealtPacket &cards)
{
	m_MyHand = cards.m_Cards;

	fill(m_PlayedCards.begin(), m_PlayedCards.end(), -1);
	m_AssetsPlayed = 0;

	// No worries about having only 5 cards: we don't play until we get 8 anyway.
	m_CardsRemainingInHand = 8;
}

void IASocket::OnPotentialAssetReceived(const std::string &assetCard)
{
	m_Asset = assetCard;
}

void IASocket::OnAskingRevealedAsset()
{
	// Count the number of cards I have at the proposed asset to deduce if it's worth trying.
	const size_t n = CountCardsForColour(m_Asset.front());
	FinalizeAssetProposal(m_Asset.front(), n);
}

void IASocket::OnAskingAnotherAsset()
{
	// Check which colours I'd be able to play
	string potentialAssets(BeloteUtils::ColourPreffixes);
	const size_t assetPos = potentialAssets.find(m_Asset.front());
	potentialAssets.erase(assetPos, 1);

	// Count the number of cards I have at each potential asset to deduce if it's worth trying.
	size_t cardsCount[3];
	size_t idx = 0;
	size_t bestOption = 0;

	for_each(potentialAssets.begin(), potentialAssets.end(),
		[&] (const char &potentialAsset)
		{
			cardsCount[idx] = CountCardsForColour(potentialAsset);
			if (cardsCount[idx] > cardsCount[bestOption])
				bestOption = idx;

			idx++;
		} );

	FinalizeAssetProposal(potentialAssets[bestOption], cardsCount[bestOption]);
}

size_t IASocket::CountCardsForColour(char col) const
{
	return accumulate(m_MyHand.begin(), m_MyHand.end(), 0,
		[&] (size_t v, const std::string &card) -> size_t
		{
			if (card.empty())
				return v;

			return v + (card.front() == col ? 1 : 0);
		} );
}

bool IASocket::HasBelote(char asset) const
{
	if (m_MyHand.end() == find(m_MyHand.begin(), m_MyHand.end(), asset + string("Q")))
		return false;

	return m_MyHand.end() != find(m_MyHand.begin(), m_MyHand.end(), asset + string("K"));
}

void IASocket::FinalizeAssetProposal(char asset, size_t cardsCountInHand)
{
	// Check probablity to try to play it
	const float probToTake[6] = {0, 0, 0.55f, 0.7f, 0.875f, 1.f };
	float p = probToTake[cardsCountInHand];
	if (HasBelote(asset))
		p += 0.15f;

	const float v = sf::Randomizer::Random(0.f, 1.f);

	// And act
	if (v > p)
		DelayReaction(boost::bind(&ClientSocket::RefuseAsset, this));
	else
		DelayReaction(boost::bind(&ClientSocket::AcceptAsset, this, string() + asset));
}

void IASocket::OnAcceptedAsset(const AcceptedAssetPacket &acceptedAsset)
{
	m_Asset = acceptedAsset.m_Asset;

	const bool botIsInNSTeam	= botInNSTeam();
	m_AssetTakenByOpponent		= botIsInNSTeam ^ acceptedAsset.m_AcceptedByNSTeam;
}

void IASocket::OnTurnStarting()
{
	// Update played card table
	int pos = 0;
	for_each(m_CurrentTurnCards.begin(), m_CurrentTurnCards.end(),
		[&] (const string &card)
		{
			if (card.empty())
				return;
			const size_t colIdx = BeloteUtils::ColourPreffixes.find(card.front());
			assert(colIdx < 4);

			const int cardIdx = BeloteUtils::GetCardIndex(card, m_Asset);
			m_PlayedCards[colIdx * 8 + cardIdx] = pos++;
		} );

	// Reset turn related states
	fill(m_CurrentTurnCards.begin(), m_CurrentTurnCards.end(), "");
	m_FirstCardThisTurn.swap(string());
}

void IASocket::OnPlayedCard(const PlayedCardPacket &playedCard)
{
	// Update turn state
	m_CurrentTurnCards[playedCard.m_Player] = playedCard.m_Card;

	if (m_FirstCardThisTurn.empty())
		m_FirstCardThisTurn = playedCard.m_Card;

	// Update play state
	if (playedCard.m_Card.front() == m_Asset.front())
		m_AssetsPlayed++;

	// And update my hand if it's me playing
	if (playedCard.m_Player == m_MySeat)
	{
		PlayerHand::iterator it = find(m_MyHand.begin(), m_MyHand.end(), playedCard.m_Card);
		assert(it != m_MyHand.end());

		swap(*it, string());
		m_CardsRemainingInHand--;
	}
}

void IASocket::OnWaitingPlay(const WaitingPlayPacket &waitingPlay)
{
	string cardToPlay;

	if (IsFirstPlayingInTurn())
	{
		cardToPlay = GetBestOpener();
		assert(!cardToPlay.empty());
	}
	else
	{
		cardToPlay = GetStrongestCardForColour(m_FirstCardThisTurn, waitingPlay.m_PossibleCards);
		if (cardToPlay.empty() || !IsCardOwningTurn(cardToPlay))
		{
			cardToPlay = GetWeakestCardForColour(m_FirstCardThisTurn, waitingPlay.m_PossibleCards);
			if (cardToPlay.empty())
			{
				string normalColours(BeloteUtils::ColourPreffixes);

				// Check if cutting is required
				const int assetsInHand	= CountCardsForColour(m_Asset.front());
				if (IsPartnerOwningTurn() && m_CardsRemainingInHand != assetsInHand)
				{
					const size_t assetPos = normalColours.find(m_Asset.front());
					normalColours.erase(assetPos, 1);
				}

				CardDefToScore scoreFunc;
				size_t minScore = 0;

				for_each(normalColours.begin(), normalColours.end(),
					[&] (const char &colour)
					{
						const string card = GetWeakestCardForColour(string() + colour, waitingPlay.m_PossibleCards);
						if (card.empty())
							return;

						if (cardToPlay.empty())
						{
							cardToPlay	= card;
							minScore	= scoreFunc(m_Asset, cardToPlay);
							return;
						}

						const size_t score	= scoreFunc(m_Asset, card);
						if (score < minScore)
							cardToPlay	= card;
					} );
			}
		}
	}

	assert(!cardToPlay.empty());
	DelayReaction(boost::bind(&ClientSocket::PlayCard, this, cardToPlay));
}

bool IASocket::IsFirstPlayingInTurn() const
{
	return m_FirstCardThisTurn.empty();
}

bool IASocket::IsPartnerOwningTurn() const
{
	const string &partnerCard = m_CurrentTurnCards[m_PartnerSeat];
	if (partnerCard.empty())
		return false;
	
	Scores scores;
	fill(scores.begin(), scores.end(), 0);
	transform(	m_CurrentTurnCards.begin(), m_CurrentTurnCards.end(),
				boost_array_iterator(scores),
				CardDefToValue(m_Asset, m_FirstCardThisTurn));
	
	return scores[m_PartnerSeat] == *std::max_element(scores.begin(), scores.end());
}

string IASocket::GetBestOpener() const
{
	string cardToPlay;

	if (ShouldPlayAsset())
	{
		// Find my highest card (ordering was performed server-side when cards were dealt)
		cardToPlay = GetStrongestCardForColour(m_Asset, m_MyHand);
		assert(!cardToPlay.empty());

		if (!IsCardOwningTurn(cardToPlay))
		{
			cardToPlay = GetWeakestCardForColour(m_Asset, m_MyHand);
			assert(!cardToPlay.empty());
		}
	}
	else // Play first card that is owning the turn or the weakest one.
	{
		string normalColours(BeloteUtils::ColourPreffixes);
		const size_t assetPos = normalColours.find(m_Asset.front());
		normalColours.erase(assetPos, 1);

		// Check if one my cards should own the turn
		string::const_iterator it = find_if(normalColours.begin(), normalColours.end(),
			[&] (const char &colour) -> bool
			{
				cardToPlay = GetStrongestCardForColour(string() + colour, m_MyHand);
				if (cardToPlay.empty())
					return false;

				return IsCardOwningTurn(cardToPlay);
			} );
			
		// No best card, check which one will make me lose the less points
		if (it == normalColours.end())
		{
			CardDefToScore scoreFunc;
			size_t minScore = numeric_limits<size_t>::max();

			for_each(normalColours.begin(), normalColours.end(),
				[&] (const char &colour)
				{
					const string card = GetWeakestCardForColour(string() + colour, m_MyHand);
					if (card.empty())
						return;

					if (cardToPlay.empty())
					{
						cardToPlay	= card;
						minScore	= scoreFunc(m_Asset, cardToPlay);
						return;
					}

					const size_t score	= scoreFunc(m_Asset, card);
					if (score < minScore)
						cardToPlay	= card;
				} );
		}
	}

	return cardToPlay;
}

bool IASocket::ShouldPlayAsset() const
{
	const int assetsInHand	= CountCardsForColour(m_Asset.front());
	if (m_CardsRemainingInHand == assetsInHand)
		return true;

	const int assetsMissing	= 8 - assetsInHand - m_AssetsPlayed;
	return !m_AssetTakenByOpponent && assetsInHand > 0 && assetsMissing > 0;
}

string IASocket::GetStrongestCardForColour(const string &colour, const PlayerHand &cardsToCheck) const
{
	PlayerHand::const_reverse_iterator it =
		find_if(cardsToCheck.rbegin(), cardsToCheck.rend(),
			[&] (const string &c) -> bool
			{
				if (c.empty()) return false;
				return c.front() == colour.front();
			} );

	if (it == cardsToCheck.rend())
		return "";

	return *it;
}

string IASocket::GetWeakestCardForColour(const string &colour, const PlayerHand &cardsToCheck) const
{
	PlayerHand::const_iterator it =
		find_if(cardsToCheck.begin(), cardsToCheck.end(),
			[&] (const string &c) -> bool
			{
				if (c.empty()) return false;
				return c.front() == colour.front();
			} );

	if (it == cardsToCheck.end())
		return "";

	return *it;
}

bool IASocket::IsCardOwningTurn(const std::string & card) const
{
	// First, look into pastly played cards to check if stronger cards may be played
	const int colourIndex			= BeloteUtils::ColourPreffixes.find(card.front());
	const size_t higherCardIndex	= colourIndex * 8 + BeloteUtils::GetCardIndex(card, m_Asset) + 1;
	if (higherCardIndex == m_PlayedCards.size())
		return true;

	DeckPlayed::const_iterator first	= m_PlayedCards.begin() + higherCardIndex;
	DeckPlayed::const_iterator last		= m_PlayedCards.begin() + higherCardIndex + (colourIndex + 1) * 8 - higherCardIndex;
	if (last > m_PlayedCards.end())
		last = m_PlayedCards.end();

	DeckPlayed::const_iterator it = find_if(first, last, [] (int c) -> bool { return c == -1; } );
	if (it != last)
		return false;

	// Check if an opponent has played a stronger card this turn
	if (m_FirstCardThisTurn.empty())
		return true;

	CardDefToValue evalFunc(m_Asset, m_FirstCardThisTurn);
	const size_t cardValue = evalFunc(card);

	Scores scores;
	fill(scores.begin(), scores.end(), 0);
	transform(	m_CurrentTurnCards.begin(), m_CurrentTurnCards.end(),
				boost_array_iterator(scores),
				evalFunc);

	return count_if(scores.begin(), scores.end(), bind2nd(greater<size_t>(), cardValue)) == 0;
}
