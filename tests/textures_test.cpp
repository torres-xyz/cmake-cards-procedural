#include <catch2/catch_test_macros.hpp>
#include "../src/textures.hpp"
#include "../src/card.hpp"

TEST_CASE("GetCardArtTexture returns a valid Texture")
{
    const raylib::Window window(0, 0, "Tests");

    REQUIRE(GetCardArtTexture(CardID::cancerPagurus).IsValid());
}

TEST_CASE("GetCardArtTexture(CardID cardId) returns a Texture")
{
    const raylib::Window window(0, 0, "Tests");

    REQUIRE(GetCardArtTexture(CardID::cancerPagurus).IsValid());
}


TEST_CASE("Card Textures have the correct dimensions")
{
    const raylib::Window window(0, 0, "Tests");

    REQUIRE(GetTexture(GameTexture::cardFrame_Unit_Form).GetHeight() == constants::cardTextureHeight);
    REQUIRE(GetTexture(GameTexture::cardFrame_Unit_Form).GetWidth() == constants::cardTextureWidth);

    REQUIRE(GetTexture(GameTexture::cardFrame_Unit_Flow).GetHeight() == constants::cardTextureHeight);
    REQUIRE(GetTexture(GameTexture::cardFrame_Unit_Flow).GetWidth() == constants::cardTextureWidth);

    REQUIRE(GetTexture(GameTexture::cardFrame_Unit_Strategy).GetHeight() == constants::cardTextureHeight);
    REQUIRE(GetTexture(GameTexture::cardFrame_Unit_Strategy).GetWidth() == constants::cardTextureWidth);

    REQUIRE(GetTexture(GameTexture::cardFrame_Unit_Instinct).GetHeight() == constants::cardTextureHeight);
    REQUIRE(GetTexture(GameTexture::cardFrame_Unit_Instinct).GetWidth() == constants::cardTextureWidth);

    REQUIRE(GetTexture(GameTexture::cardFrame_Unit_Hope).GetHeight() == constants::cardTextureHeight);
    REQUIRE(GetTexture(GameTexture::cardFrame_Unit_Hope).GetWidth() == constants::cardTextureWidth);

    REQUIRE(GetTexture(GameTexture::cardFrame_Unit_Despair).GetHeight() == constants::cardTextureHeight);
    REQUIRE(GetTexture(GameTexture::cardFrame_Unit_Despair).GetWidth() == constants::cardTextureWidth);
}
