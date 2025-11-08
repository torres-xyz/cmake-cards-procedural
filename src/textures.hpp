#pragma once
#include "raylib-cpp.hpp"

enum class CardBanner;
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
    scissorsCard,
    cardFrame_Form,
    cardFrame_Flow,
    cardFrame_Strategy,
    cardFrame_Instinct,
    cardFrame_Hope,
    cardFrame_Despair
};

raylib::Texture2D const &GetTexture(GameTexture tex);

raylib::Texture2D const &GetTexture(CardType type);

raylib::Texture2D const &GetTexture(CardBanner banner);

GameTexture GetGameTextureFromCardType(CardType type);

GameTexture GetGameTextureFromBannerType(CardBanner banner);
