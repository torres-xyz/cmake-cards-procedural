#include "game_rules.hpp"
#include "card.hpp"
#include "game_play_phases.hpp"
#include "player.hpp"

struct Card;

bool CanCardBePlayedByPlayer(const Card &selectedCard, const Player &player, [[maybe_unused]] const GameplayPhase &gameplayPhase)
{
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
