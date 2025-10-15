#pragma once

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

void PlayMusic(GameMusic music);
