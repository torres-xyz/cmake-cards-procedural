#include "../src/audio.hpp"
#include "raylib-cpp.hpp"

#include <Sound.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Sound should play", "[audio]")
{
    const raylib::Window window(0, 0, "Tests");
    InitAudioDevice();

    const raylib::Sound testSound = raylib::Sound("resources/audio/sounds/card_place_01.ogg");

    PlaySound(GameSound::cardPlace01);
    REQUIRE(IsSoundPlaying(testSound));
}
