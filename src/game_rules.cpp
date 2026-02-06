#include "game_rules.hpp"
#include <algorithm>
#include <cassert>
#include "card.hpp"
#include "game_status.hpp"
#include "player.hpp"

struct Card;

void ShuffleDeckAndMakeSureTopCardIsAUnit(std::vector<Card> &deck, std::random_device &rd)
{
    std::ranges::shuffle(deck, rd);

    if (deck.at(0).type == CardType::unit) return;

    for (std::size_t i = 0; i < deck.size(); ++i)
    {
        if (deck.at(i).type == CardType::unit)
        {
            std::swap(deck[i], deck[0]);
            break;
        }
    }

    assert(deck.at(0).type == CardType::unit);
}

CardStats CalculateCardStackTotalStats(const std::vector<Card> &stack)
{
    CardStats totalCardStats{0, 0, 0};

    for (const auto &card: stack)
    {
        totalCardStats.body += card.stats.body;
        totalCardStats.mind += card.stats.mind;
        totalCardStats.soul += card.stats.soul;
    }

    if (stack.empty()) return totalCardStats;

    if (stack.at(0).cardID == 1)
    {
        for (const auto &card: stack)
        {
            if (card.cardID != 1 && card.banner == CardBanner::form)
            {
                totalCardStats.body += 500;
            }
        }
    }

    return totalCardStats;
}

/**
 * @return 1 if Player 1 wins, 2 if player 2 wins or 0 if it's a tie.
 */
int CalculateRoundWinnerId(const Player &playerA, const Player &playerB)
{
    const CardStats totalCardStatsA = CalculateCardStackTotalStats(playerA.cardsInPlayStack);

    int maxStatA = totalCardStatsA.body > totalCardStatsA.mind ? totalCardStatsA.body : totalCardStatsA.mind;
    maxStatA = totalCardStatsA.soul > maxStatA ? totalCardStatsA.soul : maxStatA;

    const CardStats totalCardStatsB = CalculateCardStackTotalStats(playerB.cardsInPlayStack);

    int maxStatB = totalCardStatsB.body > totalCardStatsB.mind ? totalCardStatsB.body : totalCardStatsB.mind;
    maxStatB = totalCardStatsB.soul > maxStatB ? totalCardStatsB.soul : maxStatB;

    //Decide the winner
    if (maxStatA > maxStatB)
    {
        return playerA.id;
    }
    if (maxStatB > maxStatA)
    {
        return playerB.id;;
    }
    //Draw
    return 0;
}

bool IsInitialHandValid(const std::vector<Card> &playerHand, const GameRules gameRules)
{
    if (static_cast<int>(playerHand.size()) < gameRules.initialHandSize)
    {
        return false;
    }

    //An initial hand needs to have only one Unit.
    for (const Card &card: playerHand)
    {
        if (card.type == CardType::unit) return true;
    }

    return false;
}

int HasAPlayerWon(const GameStatus &gameStatus, const GameRules &gameRules)
{
    if (gameStatus.pointsPlayer1 >= gameRules.pointsNeededToWin)
    {
        return 1;
    }
    if (gameStatus.pointsPlayer2 >= gameRules.pointsNeededToWin)
    {
        return 2;
    }
    return 0;
}
