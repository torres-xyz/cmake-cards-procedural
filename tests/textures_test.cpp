#include <cstdlib>
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
