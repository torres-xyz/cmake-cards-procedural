#include "player.hpp"
#include <algorithm>
#include <random>
#include "constants.hpp"

void InitializePlayer(Player &player, std::random_device &rd)
{
    player.score = 0;
    player.hand.clear();
    //Use default deck for players for now.
    player.deck.clear();
    for (const int cardType: constants::defaultDeck)
    {
        player.deck.emplace_back(Card{.type = static_cast<CardType>(cardType)});
    }

    std::ranges::shuffle(player.deck, rd);
}
