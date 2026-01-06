#include "game_rules.hpp"

#include <cassert>

#include "card.hpp"
#include "game_play_phases.hpp"
#include "player.hpp"

struct Card;

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

int CalculateRoundWinner(const std::vector<Card> &stack1, const std::vector<Card> &stack2)
{
    const int totalBody1 = GetCardStackTotalBody(stack1);
    const int totalMind1 = GetCardStackTotalMind(stack1);
    const int totalSoul1 = GetCardStackTotalSoul(stack1);

    int maxStat1 = totalBody1 > totalMind1 ? totalBody1 : totalMind1;
    maxStat1 = totalSoul1 > maxStat1 ? totalSoul1 : maxStat1;

    const int totalBody2 = GetCardStackTotalBody(stack2);
    const int totalMind2 = GetCardStackTotalMind(stack2);
    const int totalSoul2 = GetCardStackTotalSoul(stack2);

    int maxStat2 = totalBody2 > totalMind2 ? totalBody2 : totalMind2;
    maxStat2 = totalSoul2 > maxStat2 ? totalSoul2 : maxStat2;

    //Decide the winner
    if (maxStat1 > maxStat2)
    {
        return 1;
    }
    if (maxStat2 > maxStat1)
    {
        return 2;
    }
    //Draw
    return 0;
}

bool CanCardBePlayedByPlayer(const Card &selectedCard, const Player &player, [[maybe_unused]] const GameplayPhase &gameplayPhase)
{
    //Only 1 card can be played per turn.

    // Playstack is empty ------------------------------------------------------
    // The first card in the playstack has to be a Unit.
    if (player.cardsInPlayStack.empty())
    {
        if (selectedCard.type == CardType::unit) return true;
        return false;
    }

    // Playstack is NOT empty --------------------------------------------------
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

bool IsInitialHandValid(const Player &player)
{
    //An initial hand needs to have only one Unit.
    for (const Card &card: player.hand)
    {
        if (card.type == CardType::unit) return true;
    }

    return false;
}
