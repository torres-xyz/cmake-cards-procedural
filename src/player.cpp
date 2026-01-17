#include "player.hpp"
#include <algorithm>
#include <random>
#include "constants.hpp"
#include "helper_functions.hpp"

void InitializePlayerWithAdvancedDeck(Player &player, std::random_device &rd)
{
    player.score = 0;
    player.hand.clear();
    player.cardsInPlayStack.clear();
    player.isHoldingACard = false;
    player.heldCardIndex = -1;
    player.hoveredCardUid = 0;
    player.isHoveringOverACard = false;
    //Use default deck for players for now.
    player.deck.clear();
    player.cardsPlayed = 0;

    for (int i = 0; i < constants::initialDeckSize - 3; ++i)
    {
        Card cancerPagurusCard = GetCardFromDB(StringToCardId("Cancer Pagurus"));

        player.deck.emplace_back(cancerPagurusCard);
    }
    for (int i = 0; i < 3; ++i)
    {
        Card bodyPlusCard = GetCardFromDB(StringToCardId("Body Plus"));

        player.deck.emplace_back(bodyPlusCard);
    }

    std::ranges::shuffle(player.deck, rd);
}
