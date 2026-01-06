#pragma once
#include "card.hpp"
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
    cardFrame_Unit_Form,
    cardFrame_Unit_Flow,
    cardFrame_Unit_Strategy,
    cardFrame_Unit_Instinct,
    cardFrame_Unit_Hope,
    cardFrame_Unit_Despair,
    cardFrame_Action_Form,
};

raylib::Texture2D const &GetTexture(GameTexture tex);

raylib::Texture2D const &GetTexture(CardBanner banner, CardType cardType);

raylib::Texture2D const &GetCardArtTexture(CardID cardId);
