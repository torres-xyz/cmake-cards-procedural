#include <catch2/catch_test_macros.hpp>
#include "../src/textures.hpp"
#include "../src/card.hpp"

TEST_CASE("GetGameTextureFromCardType returns Texture", "[textures]")
{
    REQUIRE(GetGameTextureFromCardType(CardType::paper) == GameTexture::paperCard);
    REQUIRE(GetGameTextureFromCardType(CardType::rock) == GameTexture::rockCard);
    REQUIRE(GetGameTextureFromCardType(CardType::scissors) == GameTexture::scissorsCard);
    REQUIRE(GetGameTextureFromCardType(CardType::invalid) == GameTexture::invalid);
}

TEST_CASE("GetTexture(CardType type) returns a Texture", "[textures]")
{
    const raylib::Window window(0, 0, "Tests");

    REQUIRE(GetTexture(CardType::paper).GetId() == GetTexture(GameTexture::paperCard).GetId());
    REQUIRE(GetTexture(CardType::rock).GetId() == GetTexture(GameTexture::rockCard).GetId());
    REQUIRE(GetTexture(CardType::scissors).GetId() == GetTexture(GameTexture::scissorsCard).GetId());
}

TEST_CASE("Card Textures have the correct dimensions")
{
    const raylib::Window window(0, 0, "Tests");

    REQUIRE(GetTexture(CardBanner::form).GetHeight() == constants::cardTextureHeight);
    REQUIRE(GetTexture(CardBanner::form).GetWidth() == constants::cardTextureWidth);

    REQUIRE(GetTexture(CardBanner::flow).GetHeight() == constants::cardTextureHeight);
    REQUIRE(GetTexture(CardBanner::flow).GetWidth() == constants::cardTextureWidth);

    REQUIRE(GetTexture(CardBanner::strategy).GetHeight() == constants::cardTextureHeight);
    REQUIRE(GetTexture(CardBanner::strategy).GetWidth() == constants::cardTextureWidth);

    REQUIRE(GetTexture(CardBanner::instinct).GetHeight() == constants::cardTextureHeight);
    REQUIRE(GetTexture(CardBanner::instinct).GetWidth() == constants::cardTextureWidth);

    REQUIRE(GetTexture(CardBanner::hope).GetHeight() == constants::cardTextureHeight);
    REQUIRE(GetTexture(CardBanner::hope).GetWidth() == constants::cardTextureWidth);

    REQUIRE(GetTexture(CardBanner::despair).GetHeight() == constants::cardTextureHeight);
    REQUIRE(GetTexture(CardBanner::despair).GetWidth() == constants::cardTextureWidth);
}
