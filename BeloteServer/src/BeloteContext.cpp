#include <iostream>
#include <algorithm>
#include <bitset>
#include <cassert>
#include <functional>
#include <iterator>

#include <SFML/System.hpp>

#include "Tools.h"
#include "typedefs.h"

#include "BeloteContext.h"
#include "Packets.h"
#include "BeloteContextPackets.h"
#include "Server.h"

const std::string					BeloteContext::ValueOrder("789JQK101");
const std::string					BeloteContext::ValueOrderAtAsset("78QK1019J");
const BeloteContext::StringArray5	BeloteContext::PlayerPositionStrings = { "South", "West", "North", "East", "Unknown" };

BeloteContext::BeloteContext(ServerPtr server)
{
	d = ContextDataPtr(new ContextData());

	d->m_Server = server;

	d->m_UnplacedPlayers.reserve(_PP_Count);
	d->m_Players.resize(_PP_Count);

	Reset();
}

BeloteContext::~BeloteContext()
{
}

void BeloteContext::Reset()
{
	auto resetPlayerPtr = [] (ServerSocketPtr p) { p.reset(); } ;

	std::for_each(d->m_UnplacedPlayers.begin(), d->m_UnplacedPlayers.end(), resetPlayerPtr);
	std::for_each(d->m_Players.begin(),d->m_Players.end(), resetPlayerPtr);
}

void BeloteContext::HandleGameContextPacket(sf::Packet &packet, ServerSocketPtr sourcePlayer)
{
	BeloteContextPacketType bcpt;
	packet >> bcpt;

	switch(bcpt)
	{
	case BCPT_PlayerChoosePos:
		{
			std::string posName;
			packet >> posName;
			SetPlayerPos(sourcePlayer, posName);
		}
		break;
					
	case BCPT_StartGameRequest:
		StartGame();
		break;

	case BCPT_AcceptAsset:
		{
			std::string colour;
			packet >> colour;
			AcceptAsset(colour);
		}
		break;
					
	case BCPT_RefuseAsset:
		RefuseAsset();
		break;

	case BCPT_PlayCard:
		{
			std::string cardName;
			packet >> cardName;
			CardPlayed(cardName);
		}
		break;

	default:
		break;
	}
}

// Player management methods
void BeloteContext::AddPlayer(ServerSocketPtr player)
{
	d->m_UnplacedPlayers.push_back(player);
	SendCurrentPositioningTo(player);
}

void BeloteContext::DropPlayer(ServerSocketPtr player)
{
	PlayersIt playerIt = std::find(d->m_Players.begin(), d->m_Players.end(), player);
	if (playerIt != d->m_Players.end())
	{
		const size_t posIdx = std::distance(d->m_Players.begin(), playerIt);
		d->m_Players[posIdx].reset();
	
		// TODO Setup phase only? replace by IA in game?
		SendCurrentPositioningToAll();
	}
	else
	{
		PlayersIt playerIt = std::find(d->m_UnplacedPlayers.begin(), d->m_UnplacedPlayers.end(), player);
		d->m_UnplacedPlayers.erase(std::remove(d->m_UnplacedPlayers.begin(), d->m_UnplacedPlayers.end(), player), d->m_UnplacedPlayers.end());
	}
}

void BeloteContext::SetPlayerPos(ServerSocketPtr player, const std::string &posName)
{
	// Find position index.
	StringArray5::const_iterator posPtr	= std::find(PlayerPositionStrings.begin(), PlayerPositionStrings.end(), posName);
	const size_t posIdx					= std::distance(PlayerPositionStrings.begin(), posPtr);

	// Place or move player
	PlayersIt playerIt = std::find(d->m_UnplacedPlayers.begin(), d->m_UnplacedPlayers.end(), player);
	if (playerIt != d->m_UnplacedPlayers.end())
	{
		d->m_UnplacedPlayers.erase(std::remove(d->m_UnplacedPlayers.begin(), d->m_UnplacedPlayers.end(), player), d->m_UnplacedPlayers.end());

		// TODO conccurrency issue lurking here! last arrived impose his will!
		d->m_Players[posIdx] = player;
	}
	else
	{
		playerIt = std::find(d->m_Players.begin(), d->m_Players.end(), player);
		if (playerIt != d->m_Players.end())
		{
			(*playerIt).reset();
			d->m_Players[posIdx] = player;
		}
		else
		{
			std::cout << "[Server] BeloteContext::SetPlayerPos: player not found!" << std::endl;
		}
	}

	SendCurrentPositioningToAll();
}

void BeloteContext::SendCurrentPositioningToAll()
{
	auto action = [&] (Players::const_reference cref) { if (cref) SendCurrentPositioningTo(cref); };

	std::for_each(d->m_UnplacedPlayers.begin(),	d->m_UnplacedPlayers.end(),	action);
	std::for_each(d->m_Players.begin(),			d->m_Players.end(),			action);
}

void BeloteContext::SendCurrentPositioningTo(ServerSocketPtr player)
{
	sf::Packet packet;
	packet << PT_GameContextPacket << BCPT_CurrentPositionning;
	std::for_each(d->m_Players.begin(), d->m_Players.end(),
			[&] (Players::const_reference cref)
			{
				if (cref)
					packet << cref->GetClientName();
				else
					packet << std::string();
			}
		);

	player->GetSocket().Send(packet);
}

// Game management methods
void BeloteContext::StartGame()
{
	NotifyStarting();

	// Init game state.
	InitDeck();
	ShuffleDeck();
	std::fill(d->m_Scores, d->m_Scores + countof(d->m_Scores), 0);

	d->m_CurrentDealer = PP_South;
	PreTurn();
}

void BeloteContext::NotifyStarting()
{
	sf::Packet packet;
	packet << PT_GameContextPacket << BCPT_GameStarting;
	std::for_each(d->m_Players.begin(), d->m_Players.end(),
			[&] (Players::const_reference cref)
			{
				cref->GetSocket().Send(packet);
			}
	);
}

void BeloteContext::InitDeck()
{
	// FIXME generate this in a smarter way maybe?
	d->m_Deck[0] = "C1";
	d->m_Deck[1] = "C7";
	d->m_Deck[2] = "C8";
	d->m_Deck[3] = "C9";
	d->m_Deck[4] = "C10";
	d->m_Deck[5] = "CJ";
	d->m_Deck[6] = "CQ";
	d->m_Deck[7] = "CK";
	d->m_Deck[8] = "D1";
	d->m_Deck[9] = "D7";
	d->m_Deck[10] = "D8";
	d->m_Deck[11] = "D9";
	d->m_Deck[12] = "D10";
	d->m_Deck[13] = "DJ";
	d->m_Deck[14] = "DQ";
	d->m_Deck[15] = "DK";
	d->m_Deck[16] = "H1";
	d->m_Deck[17] = "H7";
	d->m_Deck[18] = "H8";
	d->m_Deck[19] = "H9";
	d->m_Deck[20] = "H10";
	d->m_Deck[21] = "HJ";
	d->m_Deck[22] = "HQ";
	d->m_Deck[23] = "HK";
	d->m_Deck[24] = "S1";
	d->m_Deck[25] = "S7";
	d->m_Deck[26] = "S8";
	d->m_Deck[27] = "S9";
	d->m_Deck[28] = "S10";
	d->m_Deck[29] = "SJ";
	d->m_Deck[30] = "SQ";
	d->m_Deck[31] = "SK";
}

void BeloteContext::ShuffleDeck()
{
	// more info available @ http://www.cigital.com/papers/download/developer_gambling.php
	// of course, the "safe seed" is not so safe here, at least at the moment.
	for(size_t ct = 0; ct != d->m_Deck.size(); ct++)
	{
		int x = sf::Randomizer::Random((int)ct, (int)d->m_Deck.size() - 1);
		std::swap(d->m_Deck[ct], d->m_Deck[x]);
	}
}

void BeloteContext::PreTurn()
{
	d->m_CurrentPlayer = GetNextPlayer(d->m_CurrentDealer);

	NotifyTurnEvent(TE_Dealing, d->m_Players[d->m_CurrentDealer]);
	DealFirstPart();
	
	d->m_IsInFirstAnnouncePhase = true;
	AskForAsset();
}

void BeloteContext::DealFirstPart()
{
	// TODO customize dealing with 3-2 / 2-3
	// Only supporting 3-2 for now
	int currentCardsCountToGive	= 3;
	PlayerPosition pp		= d->m_CurrentPlayer;
	d->m_CurrentDeckPos		= 0;
	int currentHandPos[4]	= { 0, 0, 0, 0 };
	for (int playerIdx = 0; playerIdx != _PP_Count * 2; playerIdx++)
	{
		for (int card = 0; card != currentCardsCountToGive; card++)
			d->m_PlayersHand[pp][currentHandPos[pp]++] = d->m_Deck[d->m_CurrentDeckPos++];

		pp = GetNextPlayer(pp);

		if (playerIdx == _PP_Count - 1)
			currentCardsCountToGive = 5 - currentCardsCountToGive;
	}

	// Ensure that the last 3 cards are not currently dealt.
	for (int c = 0; c != 3; c++)
	{
		for (int p = 0; p != _PP_Count; p++)
			d->m_PlayersHand[p][7 - c] = "";
	}

	OrderHands();
	SendCurrentHands();

	// Show potential asset.
	d->m_PotentialAsset = d->m_Deck[d->m_CurrentDeckPos++];
	std::for_each(d->m_Players.begin(), d->m_Players.end(),
		[&] (Players::reference player)
		{
			sf::Packet packet;
			packet << PT_GameContextPacket << BCPT_PotentialAsset;
			packet << d->m_PotentialAsset;

			player->GetSocket().Send(packet);
		}
	);
}

void BeloteContext::AskForAsset()
{
	sf::Packet packet;
	packet << PT_GameContextPacket;
	if (d->m_IsInFirstAnnouncePhase)
		packet << BCPT_AskRevealedAsset;
	else
		packet << BCPT_AskAnotherAsset;

	d->m_Players[d->m_CurrentPlayer]->GetSocket().Send(packet);
}

void BeloteContext::AcceptAsset(const std::string &colourName)
{
	d->m_CurrentAsset = colourName.front();
	d->m_TeamAcceptingContract = (d->m_CurrentPlayer == PP_South || d->m_CurrentPlayer == PP_North) ? TI_NorthSouth : TI_WestEast;

	NotifyTurnEvent(TE_TakeAsset, d->m_Players[d->m_CurrentPlayer]);

	DealLastPart();
}

void BeloteContext::RefuseAsset()
{
	if (d->m_CurrentPlayer == d->m_CurrentDealer)
		d->m_IsInFirstAnnouncePhase = false;

	NotifyTurnEvent(TE_RefuseAsset, d->m_Players[d->m_CurrentPlayer]);

	d->m_CurrentPlayer = GetNextPlayer(d->m_CurrentPlayer);
	
	AskForAsset();
}

void BeloteContext::DealLastPart()
{
	PlayerPosition taker	= d->m_CurrentPlayer;
	d->m_CurrentPlayer		= GetNextPlayer(d->m_CurrentDealer);
	PlayerPosition pp		= d->m_CurrentPlayer;
	int currentHandPos[4]	= { 5, 5, 5, 5 };

	// Finish distributing the deck
	for (int i = 0; i != _PP_Count; i++)
	{
		int cardsToGive = (pp == taker) ? 2 : 3;
		for (int card = 0; card != cardsToGive; card++)
			d->m_PlayersHand[pp][currentHandPos[pp]++] = d->m_Deck[d->m_CurrentDeckPos++];

		pp = GetNextPlayer(pp);
	}

	// Give the shown card to the taker.
	d->m_PlayersHand[taker][7] = d->m_PotentialAsset;
	
	for (int i = 0; i != _PP_Count; i++)
		d->m_RemainingCards[i] = 8;

	OrderHands();
	SendCurrentHands();

	StartTurn();
}

void BeloteContext::StartTurn()
{
	NotifyTurnEvent(TE_TurnStarting);

	d->m_CurrentlyPlayedCards = 0;
	std::fill(d->m_PlayedCards.begin(), d->m_PlayedCards.end(), std::string());

	AskToPlay();
}

void BeloteContext::AskToPlay()
{
	sf::Packet packet;
	packet << PT_GameContextPacket << BCPT_WaitingPlay;

	std::vector<std::string> playableCards;
	playableCards.reserve(d->m_RemainingCards[d->m_CurrentPlayer]);

	// Search and specify which cards are playable this turn.
	if (d->m_CurrentlyPlayedCards == 0)
	{
		DumpAllCardsInHandTo(playableCards);
	}
	else
	{
		const bool hasAssetsInHand = PlayerHasColourInHand(d->m_CurrentAsset);

		if (d->m_PlayedCards[0].front() == d->m_CurrentAsset.front())
		{
			if (hasAssetsInHand)
			{
				if (PlayerCanOvercut())
				{
					DumpOvercuttingCardsTo(playableCards);
				}
				else
				{
					DumpColoursInHandTo(d->m_CurrentAsset, playableCards);
				}
			}
			else
			{
				DumpAllCardsInHandTo(playableCards);
			}
		}
		else // Request colour is not the asset
		{
			const std::string colourID(1, d->m_PlayedCards[0].front());
			if (PlayerHasColourInHand(colourID))
			{
				DumpColoursInHandTo(colourID, playableCards);
			}
			else
			{
				if (hasAssetsInHand && PlayerMustCut())
				{
					if (PlayerCanOvercut())
					{
						DumpOvercuttingCardsTo(playableCards);
					}
					else
					{
						DumpColoursInHandTo(d->m_CurrentAsset, playableCards);
					}
				}
				else
				{
					DumpAllCardsInHandTo(playableCards);
				}
			}
		}
	}

	packet << playableCards.size();
	std::for_each(playableCards.begin(), playableCards.end(), [&] (const std::string &card) { packet << card; } );

	d->m_Players[d->m_CurrentPlayer]->GetSocket().Send(packet);
}

void BeloteContext::EvaluatePlayedCards(Scores &scores) const
{
	std::fill(scores.begin(), scores.end(), 0);
	std::transform(	d->m_PlayedCards.begin(), d->m_PlayedCards.end(),
					boost_array_iterator(scores),
					std::bind1st(CardDefToScore(), d));
}

size_t BeloteContext::GetMaxScoreFromPlayedCards() const
{
	Scores playedCardScores;
	EvaluatePlayedCards(playedCardScores);

	return *std::max_element(playedCardScores.begin(), playedCardScores.end());
}

void BeloteContext::DumpAllCardsInHandTo(std::vector<std::string> &out) const
{
	std::copy(	d->m_PlayersHand[d->m_CurrentPlayer].begin(),
				d->m_PlayersHand[d->m_CurrentPlayer].begin() + d->m_RemainingCards[d->m_CurrentPlayer],
				std::back_inserter(out));
}

void BeloteContext::DumpColoursInHandTo(const std::string &colour, std::vector<std::string> &out) const
{
	std::copy_if(	d->m_PlayersHand[d->m_CurrentPlayer].begin(),
					d->m_PlayersHand[d->m_CurrentPlayer].begin() + d->m_RemainingCards[d->m_CurrentPlayer],
					std::back_inserter(out),
					std::bind1st(IsSameColour(), colour));
}

void BeloteContext::DumpOvercuttingCardsTo(std::vector<std::string> &out) const
{
	const size_t maxPlayedScore	= GetMaxScoreFromPlayedCards();

	// And use it to filter which cards from player's hand is playable.
	CardDefToScore scoringFunc;

	std::copy_if(	d->m_PlayersHand[d->m_CurrentPlayer].begin(),
					d->m_PlayersHand[d->m_CurrentPlayer].begin() + d->m_RemainingCards[d->m_CurrentPlayer],
					std::back_inserter(out),
					[&] (const std::string &card) -> bool
					{
						const size_t score = scoringFunc(d, card);
						return score > maxPlayedScore;
					} );
}

bool BeloteContext::PlayerHasColourInHand(const std::string &colour) const
{
	const std::string *result = std::find_if(	d->m_PlayersHand[d->m_CurrentPlayer].begin(),
												d->m_PlayersHand[d->m_CurrentPlayer].end(),
												std::bind1st(IsSameColour(), colour));
	return result != d->m_PlayersHand[d->m_CurrentPlayer].end();
}

bool BeloteContext::PlayerMustCut() const
{
	assert(d->m_CurrentlyPlayedCards > 0);
	assert(d->m_PlayedCards[0].front() != d->m_CurrentAsset.front());

	// If only an opponent has played, then player should cut.
	if (d->m_CurrentlyPlayedCards == 1)
		return true;
	
	Scores scores;
	EvaluatePlayedCards(scores);
	
	// Check if partner's card is not the master.
	const int partnerCardIndex	= d->m_CurrentlyPlayedCards - 2;
	return scores[partnerCardIndex] != *std::max_element(scores.begin(), scores.end());
}

bool BeloteContext::PlayerCanOvercut() const
{
	const size_t maxPlayedScore	= GetMaxScoreFromPlayedCards();
	
	Scores handScores;
	std::transform(	d->m_PlayersHand[d->m_CurrentPlayer].begin(),
					d->m_PlayersHand[d->m_CurrentPlayer].end(),
					boost_array_iterator(handScores),
					std::bind1st(CardDefToScore(), d));

	const size_t maxHandScore = *std::max_element(handScores.begin(), handScores.end());

	return maxHandScore > maxPlayedScore;
}

void BeloteContext::CardPlayed(const std::string &card)
{
	// Notify players that a card is played by current player
	sf::Packet packet;
	packet	<< PT_GameContextPacket << BCPT_CardPlayed
			<< card << d->m_CurrentPlayer;
	std::for_each(d->m_Players.begin(), d->m_Players.end(),
		[&] (Players::reference player) { player->GetSocket().Send(packet); }
	);

	// Remove card from player's hand
	// NB: the out of range index is *intentional*! We want the element past the end, to conform with STL iterators.
	PlayerHand::iterator newEnd	= std::remove(	d->m_PlayersHand[d->m_CurrentPlayer].begin(),
												d->m_PlayersHand[d->m_CurrentPlayer].end(),
												card);
	std::fill(newEnd, d->m_PlayersHand[d->m_CurrentPlayer].end(), std::string());

	// Update played cards
	d->m_RemainingCards[d->m_CurrentPlayer]--;
	d->m_PlayedCards[d->m_CurrentlyPlayedCards] = card;
	d->m_CurrentlyPlayedCards++;

	// Next player!
	if (d->m_CurrentlyPlayedCards == _PP_Count)
	{
		Scores scores;
		EvaluatePlayedCards(scores);
		const size_t maxScore = *std::max_element(scores.begin(), scores.end());

		size_t winner			= 0;
		for(; winner != 4; winner++)
		{
			if (scores[winner] == maxScore)
				break;
		}
		assert(winner < 4);

		const int winnerPos = (GetNextPlayer(d->m_CurrentPlayer) + winner) % _PP_Count;

		d->m_CurrentPlayer = static_cast<PlayerPosition>(winnerPos);

		StartTurn();
	}
	else
	{
		d->m_CurrentPlayer = GetNextPlayer(d->m_CurrentPlayer);
		AskToPlay();
	}
}

void BeloteContext::OrderHands()
{
	auto compFunc = [&](const std::string &c1, const std::string &c2) -> bool
					{
						if (c1 == "")	return false;
						if (c2 == "")	return true;
						
						static const std::string colourOrder("HSDC");
						const size_t	c1ColourIdx = colourOrder.find(c1.front()),
										c2ColourIdx = colourOrder.find(c2.front());
						if (c1ColourIdx < c2ColourIdx)	return true;
						if (c2ColourIdx < c1ColourIdx)	return false;
						
						const bool		isAsset		= (d->m_CurrentAsset != "" && d->m_CurrentAsset.front() == c1.front());
						const size_t	c1ValueIdx	= (isAsset ? ValueOrderAtAsset : ValueOrder).rfind(c1.c_str() + 1),
										c2ValueIdx	= (isAsset ? ValueOrderAtAsset : ValueOrder).rfind(c2.c_str() + 1);
						return c1ValueIdx < c2ValueIdx; // Same colour, sort by card.
					};

	for (int p = 0; p != _PP_Count; p++)
		std::sort(d->m_PlayersHand[p].begin(), d->m_PlayersHand[p].end(), compFunc);
}

void BeloteContext::NotifyTurnEvent(TurnEvent event, ServerSocketPtr player)
{
	sf::Packet packet;
	packet << PT_GameContextPacket;

	switch (event)
	{
	case TE_Dealing:
		packet << BCPT_Dealing << player->GetClientName().c_str();
		break;

	case TE_TakeAsset:
		packet << BCPT_AssetAccepted << player->GetClientName().c_str() << d->m_CurrentAsset;
		break;

	case TE_RefuseAsset:
		packet << BCPT_AssetRefused << player->GetClientName().c_str();
		break;

	case TE_TurnStarting:
		packet << BCPT_TurnStarting;
		break;
	}
	
	std::for_each(d->m_Players.begin(), d->m_Players.end(),
		[&] (Players::reference player) { player->GetSocket().Send(packet); }
	);
}

void BeloteContext::SendCurrentHands()
{
	int playerIndex = 0;
	std::for_each(d->m_Players.begin(), d->m_Players.end(),
		[&] (Players::reference player)
		{
			sf::Packet packet;
			packet << PT_GameContextPacket << BCPT_CardsDealt;
			for (int i = 0; i != 8; i++)
				packet << d->m_PlayersHand[playerIndex][i];

			player->GetSocket().Send(packet);

			playerIndex++;
		}
	);
}
