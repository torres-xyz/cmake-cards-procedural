#include <catch2/catch_test_macros.hpp>
#include "../src/player.hpp"

TEST_CASE("InitializePlayer sets player variable to empty defaults ", "[player]")
{
    std::random_device rd{};
    SetRandomSeed(rd());

    constexpr int playerID = 10;

    Player playerUnderTest
    {
        .id = playerID,
        .score = 100,
        .deck = {
            Card{.type = CardType::paper},
            Card{.type = CardType::rock},
        },
        .hand = {
            Card{.type = CardType::paper},
            Card{.type = CardType::rock},
            Card{.type = CardType::scissors},
        },
        .cardInPlay{Card{.type = CardType::paper}},
        .isHoldingACard = true,
        .heldCardIndex = 2
    };

    InitializePlayerWithDeck(playerUnderTest, rd);

    SECTION("Player ID shouldn't change")
    {
        REQUIRE(playerUnderTest.id == playerID);
    }
    SECTION("Player should have a deck")
    {
        REQUIRE(playerUnderTest.deck.empty() == false);
    }
    SECTION("Everything else should be reset")
    {
        REQUIRE(playerUnderTest.score == 0);
        REQUIRE(playerUnderTest.hand.empty() == true);
        REQUIRE(playerUnderTest.cardInPlay.type == CardType::invalid);
        REQUIRE(playerUnderTest.isHoldingACard == false);
        REQUIRE(playerUnderTest.heldCardIndex == -1);
    }
}
