#include "game_rules.hpp"
#include "card.hpp"
#include "player.hpp"

struct Card;

bool CanCardBePlayedByPlayer(const Card &selectedCard, const Player &player, [[maybe_unused]] const GameplayPhase &gameplayPhase)
{
    // The first card in the playstack has to be a Unit.
    if (player.cardsInPlayStack.empty())
    {
        if (selectedCard.type == CardType::unit) return true;
        return false;
    }

    //if the playstack is NOT empty

    //then we can assume the first/bottom card is a Unit.
    //only Action cards can be played on top of a Unit card:
    if (selectedCard.type == CardType::action) return true;


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
