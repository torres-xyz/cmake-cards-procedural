#include "../src/audio.hpp"
#include "raylib-cpp.hpp"

#include <Sound.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Sound should play", "[audio]")
{
    const raylib::Window window(0, 0, "Tests");
    InitAudioDevice();

    constexpr GameSound sound{GameSound::cardPlace01};

    PlaySound(sound);
    REQUIRE(IsSoundPlaying(GetSoundFromLibrary(sound)));
}
