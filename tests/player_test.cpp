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
            Card{.type = CardType::unit},
            Card{.type = CardType::unit},
        },
        .hand = {
            Card{.type = CardType::unit},
            Card{.type = CardType::action},
            Card{.type = CardType::unit},
        },
        .cardsInPlayStack = {
            Card{.type = CardType::unit},
            Card{.type = CardType::action},
        },
        .isHoldingACard = true,
        .heldCardIndex = 2,
        .hoveredCardUid = 2,
        .hasDrawnThisTurn = true,
        .hasEndedTheTurn = true,
        .cardsPlayed = 10
    };

    InitializePlayerWithAdvancedDeck(playerUnderTest, rd);

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
        //TODO: Finish this
        REQUIRE(playerUnderTest.score == 0);
        REQUIRE(playerUnderTest.hand.empty() == true);
        REQUIRE(playerUnderTest.isHoldingACard == false);
        REQUIRE(playerUnderTest.heldCardIndex == -1);
    }
}
