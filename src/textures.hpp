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
    wood11,
    panel01,
    panel05,
    cardBack,
    paperCard,
    rockCard,
    scissorsCard
};

raylib::Texture2D const &GetTexture(GameTexture tex);

raylib::Texture2D const &GetTexture(CardType type);

GameTexture GetGameTextureFromCardType(CardType type);
