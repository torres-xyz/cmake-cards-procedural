#include "player.hpp"
#include <algorithm>
#include <random>
#include "constants.hpp"

void InitializePlayerWithAdvancedDeck(Player &player, std::random_device &rd)
{
    player.score = 0;
    player.hand.clear();
    player.cardsInPlayStack.clear();
    player.isHoldingACard = false;
    player.heldCardIndex = -1;
    //Use default deck for players for now.
    player.deck.clear();
    player.cardsPlayed = 0;

    std::uniform_int_distribution randomRange{1, 20};

    for (int i = 0; i < constants::initialDeckSize - 3; ++i)
    {
        Card advancedCard
        {
            .rect = {100, 100, constants::cardWidth, constants::cardHeight},
            .type = CardType::unit,
            .faceUp = true,
            .id = CardID::firstCard,
            .name = "Lorem Ipsum",
            .bodyText = "Id aspernatur consequuntur eos ut quia vero. Voluptas "
            "beatae ut temporibus consectetur eveniet placeat adipisci. "
            "Dignissimos aut et recusandae voluptates harum.",
            .banner = CardBanner::form,
            .body = randomRange(rd) * 500,
            .mind = randomRange(rd) * 500,
            .soul = randomRange(rd) * 500
        };

        player.deck.emplace_back(advancedCard);
    }
    for (int i = 0; i < 3; ++i)
    {
        Card advancedActionCard
        {
            .rect = {100, 100, constants::cardWidth, constants::cardHeight},
            .type = CardType::action,
            .faceUp = true,
            .id = CardID::firstCardAction,
            .name = "Action Card One",
            .bodyText = "+1000 Body to your Unit.",
            .banner = CardBanner::form,
            .body = -1,
            .mind = -1,
            .soul = -1
        };

        player.deck.emplace_back(advancedActionCard);
    }

    std::ranges::shuffle(player.deck, rd);
}
