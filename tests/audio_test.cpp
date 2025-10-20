#include "../src/audio.hpp"

#include <Sound.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Sound should play", "[audio]")
{
    raylib::Sound testSound = raylib::Sound("resources/audio/sounds/card_place_01.ogg");

    PlaySound(GameSound::cardPlace01);
    REQUIRE(IsSoundPlaying(testSound));
}
