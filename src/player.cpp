#include "player.hpp"
#include <algorithm>
#include <random>
#include "constants.hpp"

void InitializePlayerWithDeck(Player &player, std::random_device &rd)
{
    player.score = 0;
    player.hand.clear();
    player.cardInPlay.type = CardType::invalid;
    player.isHoldingACard = false;
    player.heldCardIndex = -1;
    //Use default deck for players for now.
    player.deck.clear();
    for (const int cardType: constants::defaultDeck)
    {
        player.deck.emplace_back(Card{.type = static_cast<CardType>(cardType)});
    }

    std::ranges::shuffle(player.deck, rd);
}
