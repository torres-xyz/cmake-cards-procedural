#pragma once
#include "raylib-cpp.hpp"

enum class CardType;

enum class GameTexture
{
    invalid,
    metal08,
    metal22,
    metal35,
    wall13,
    panel01,
    panel05,
    cardBack,
    paperCard,
    rockCard,
    scissorsCard
};

raylib::Texture2D &GetTexture(GameTexture tex);

GameTexture GetGameTextureFromCardType(CardType type);
