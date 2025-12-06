#include "player.hpp"
#include <algorithm>
#include <random>
#include "constants.hpp"

void InitializePlayerWithAdvancedDeck(Player &player, std::random_device &rd)
{
    player.score = 0;
    player.hand.clear();
    player.cardInPlay.type = CardType::invalid;
    player.isHoldingACard = false;
    player.heldCardIndex = -1;
    //Use default deck for players for now.
    player.deck.clear();

    std::uniform_int_distribution randomRange{1, 20};


    for (int i = 0; i < constants::initialDeckSize; ++i)
    {
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
            .body = randomRange(rd) * 500,
            .mind = randomRange(rd) * 500,
            .soul = randomRange(rd) * 500
        };

        player.deck.emplace_back(advancedCard);
    }

    std::ranges::shuffle(player.deck, rd);
}
