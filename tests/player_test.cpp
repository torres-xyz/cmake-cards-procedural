#include <catch2/catch_test_macros.hpp>
#include "../src/player.hpp"

#include "../src/helper_functions.hpp"

TEST_CASE("InitializePlayer sets player variable to empty defaults ", "[player]")
{
    constexpr int playerID = 1;

    Player playerUnderTest
    {
        .id = playerID,
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
        .heldCardUid = 2,
        .hoveredCardUid = 2,
    };

    InitializePlayer(playerUnderTest);

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
        REQUIRE(playerUnderTest.hand.empty() == true);
        REQUIRE(playerUnderTest.isHoldingACard == false);
        REQUIRE(playerUnderTest.cardsInPlayStack.empty() == true);
        REQUIRE(playerUnderTest.availableActions.empty() == true);
        REQUIRE(playerUnderTest.chosenAction.action == PlayerAction::null);
        REQUIRE(playerUnderTest.isHoldingACard == false);
        REQUIRE(playerUnderTest.heldCardUid == 0);
        REQUIRE(playerUnderTest.hoveredCardUid == 0);
        REQUIRE(playerUnderTest.isHoveringOverACard == false);
    }
}
