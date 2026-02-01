#include "game_rules.hpp"
#include <algorithm>
#include <cassert>
#include "card.hpp"
#include "game_play_phases.hpp"
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

int GetCardStackTotalBody(const std::vector<Card> &stack)
{
    int bodyStatTotal{0};

    for (const auto &card: stack)
    {
        bodyStatTotal += card.body;
    }

    return bodyStatTotal;
}

int GetCardStackTotalMind(const std::vector<Card> &stack)
{
    int mindStatTotal{0};

    for (const auto &card: stack)
    {
        mindStatTotal += card.mind;
    }

    return mindStatTotal;
}

int GetCardStackTotalSoul(const std::vector<Card> &stack)
{
    int soulStatTotal{0};

    for (const auto &card: stack)
    {
        soulStatTotal += card.soul;
    }

    return soulStatTotal;
}

/**
 * @return 1 if Player 1 wins, 2 if player 2 wins or 0 if it's a tie.
 */
int CalculateRoundWinnerId(const Player &playerA, const Player &playerB)
{
    const int totalBodyA = GetCardStackTotalBody(playerA.cardsInPlayStack);
    const int totalMindA = GetCardStackTotalMind(playerA.cardsInPlayStack);
    const int totalSoulA = GetCardStackTotalSoul(playerA.cardsInPlayStack);

    int maxStatA = totalBodyA > totalMindA ? totalBodyA : totalMindA;
    maxStatA = totalSoulA > maxStatA ? totalSoulA : maxStatA;

    const int totalBodyB = GetCardStackTotalBody(playerB.cardsInPlayStack);
    const int totalMindB = GetCardStackTotalMind(playerB.cardsInPlayStack);
    const int totalSoulB = GetCardStackTotalSoul(playerB.cardsInPlayStack);

    int maxStatB = totalBodyB > totalMindB ? totalBodyB : totalMindB;
    maxStatB = totalSoulB > maxStatB ? totalSoulB : maxStatB;

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

bool CanCardBePlayedByPlayer(const Card &selectedCard, const Player &player, [[maybe_unused]] const GameplayPhase &gameplayPhase)
{
    //Only 1 card can be played per turn.

    // Play Stack is empty ------------------------------------------------------
    // The first card in the play stack has to be a Unit.
    if (player.cardsInPlayStack.empty())
    {
        if (selectedCard.type == CardType::unit) return true;
        return false;
    }

    // Play stack is NOT empty --------------------------------------------------
    //On the first turn, players can only play 1 unit.

    if (gameplayPhase == GameplayPhase::playerOneFirstTurn)
    {
        return false;
    }

    //then we can assume (but still verify) the first/bottom card is a Unit.
    if (player.cardsInPlayStack.at(0).type == CardType::unit)
    {
        //only Action cards can be played on top of a Unit card:
        if (selectedCard.type == CardType::action) return true;
    }

    return false;
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

int HasAPlayerWon([[maybe_unused]] const Player &playerA, [[maybe_unused]] const Player &playerB, const GameStatus &gameStatus, const GameRules &gameRules)
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
