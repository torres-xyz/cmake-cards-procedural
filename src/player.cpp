#include "player.hpp"
#include <algorithm>
#include <random>
#include "helper_functions.hpp"

void InitializePlayer(Player &player)
{
    player.hand.clear();
    player.cardsInPlayStack.clear();
    player.isHoldingACard = false;
    player.heldCardUid = 0;
    player.hoveredCardUid = 0;
    player.isHoveringOverACard = false;
    //Use default deck for players for now.
    player.deck.clear();
    player.cardsPlayed = 0;

    if (player.id == 1)
    {
        for (const int deck1Card: deckP1)
        {
            player.deck.push_back(GetCardFromDB(deck1Card));
        }
    }
    if (player.id == 2)
    {
        for (const int deck2Card: deckP2)
        {
            player.deck.push_back(GetCardFromDB(deck2Card));
        }
    }

    std::ranges::shuffle(player.deck, HelperFunctions::GetRandomDevice());
}
