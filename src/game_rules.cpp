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

CardStats CalculateCardStackTotalStats(const Player &player, const GameStatus &gameStatus)
{
    const std::vector<Card> &stack = player.cardsInPlayStack;
    CardStats totalCardStats{0, 0, 0};
    if (stack.empty()) return totalCardStats;

    for (size_t i = 0; i < stack.size(); ++i)
    {
        totalCardStats.body += stack.at(i).stats.body;
        totalCardStats.mind += stack.at(i).stats.mind;
        totalCardStats.soul += stack.at(i).stats.soul;

        switch (stack.at(i).cardID)
        {
            case 1: //Cancer Pagurus: "+500 to Body for each Form Action on this Unit."
            {
                for (const auto &card: stack)
                {
                    if (card.cardID != 1 && card.banner == CardBanner::form)
                    {
                        totalCardStats.body += 500;
                    }
                }
                break;
            }
            case 4: //Twisted Gauntlet: "+2000 to Body if you have lost the previous round."
            {
                if (!gameStatus.roundWinnerHistory.empty() && gameStatus.roundWinnerHistory.back() != player.id)
                {
                    totalCardStats.body += 2000;
                }
                break;
            }
            default: ;
        }
    }

    return totalCardStats;
}

/**
 * @return 1 if Player 1 wins, 2 if player 2 wins or 0 if it's a tie.
 */
int CalculateRoundWinnerId(const Player &playerA, const Player &playerB, const GameStatus &gameStatus)
{
    const CardStats totalCardStatsA = CalculateCardStackTotalStats(playerA, gameStatus);

    int maxStatA = totalCardStatsA.body > totalCardStatsA.mind ? totalCardStatsA.body : totalCardStatsA.mind;
    maxStatA = totalCardStatsA.soul > maxStatA ? totalCardStatsA.soul : maxStatA;

    const CardStats totalCardStatsB = CalculateCardStackTotalStats(playerB, gameStatus);

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

    //A valid initial hand needs to have at least one Unit.
    return std::ranges::any_of(playerHand, [](const Card &card) { return card.type == CardType::unit; });

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
