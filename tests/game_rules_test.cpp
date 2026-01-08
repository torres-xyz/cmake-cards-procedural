#include <catch2/catch_test_macros.hpp>
#include "../src/game_rules.hpp"
#include "../src/card.hpp"
#include "raylib.h"


TEST_CASE("ShuffleDeckAndMakeSureTopCardIsAUnit leaves a Card of type Unit at index 0")
{
    //Arrange
    std::random_device rd{};
    SetRandomSeed(rd());

    //Fill a deck with 30 action cards and 1 Unit card
    std::vector<Card> testDeck{};
    constexpr int deckSize{30};

    for (int i = 0; i < deckSize; ++i)
    {
        Card actionCard
        {
            .type = CardType::action,
        };

        testDeck.emplace_back(actionCard);
    }
    const Card unitCard
    {
        .type = CardType::unit,
    };
    testDeck.emplace_back(unitCard);

    //Act
    ShuffleDeckAndMakeSureTopCardIsAUnit(testDeck, rd);

    //Assert
    REQUIRE(testDeck.at(0).type == CardType::unit);
}
