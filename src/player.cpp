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

void InitializePlayerWithAdvancedDeck(Player &player, std::random_device &rd)
{
    player.score = 0;
    player.hand.clear();
    player.cardInPlay.type = CardType::invalid;
    player.isHoldingACard = false;
    player.heldCardIndex = -1;
    //Use default deck for players for now.
    player.deck.clear();

    Card advancedCard
    {
        .rect = {100, 100, constants::cardWidth, constants::cardHeight},
        .type = CardType::prototypeCard,
        .faceUp = true,
        .id = CardID::firstCard,
        .name = "Lorem Ipsum",
        .bodyText = "Id aspernatur consequuntur eos ut quia vero. Voluptas "
        "beatae ut temporibus consectetur eveniet placeat adipisci. "
        "Dignissimos aut et recusandae voluptates harum. Enim non et "
        "facilis. Nemo reiciendis dolores dolores illum omnis.",
        .banner = CardBanner::form,
        .body = 3000,
        .mind = 2000,
        .soul = 1000
    };

    for (int i = 0; i < constants::initialDeckSize; ++i)
    {
        player.deck.emplace_back(advancedCard);
    }

    std::ranges::shuffle(player.deck, rd);
}
