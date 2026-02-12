#include "player.hpp"
#include <algorithm>
#include <random>
#include "helper_functions.hpp"

void InitializePlayer(Player &player, const std::vector<int> &deckList)
{
    player.hand.clear();
    player.cardsInPlayStack.clear();
    player.isHoldingACard = false;
    player.heldCardUid = 0;
    player.hoveredCardUid = 0;
    player.isHoveringOverACard = false;
    //Use default deck for players for now.
    player.deck.clear();

    for (const int card: deckList)
    {
        player.deck.push_back(GetCardFromDB(card));
    }

    std::ranges::shuffle(player.deck, HelperFunctions::GetRandomDevice());
}


/**
 * Best used before the initial draw.
 *
 */
void DEBUG_InjectDeckInPlayer(Player &player, const std::vector<int> &deck)
{
    player.deck.clear();

    for (const int cardID: deck)
    {
        player.deck.push_back(GetCardFromDB(cardID));
    }
}
