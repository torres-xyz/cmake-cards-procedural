#pragma once
#include <map>
#include <string>
#include "raylib-cpp.hpp"


enum class GameSound
{
    buttonPress01,
    cardPlace01,
};

enum class GameMusic
{
    start,
    playing,
    gameOver
};

void PlaySound(GameSound sound);

raylib::Sound &GetSoundFromLibrary(GameSound sound);

void PlayMusic(GameMusic music);

raylib::Music &GetMusicFromLibrary(GameMusic music);


