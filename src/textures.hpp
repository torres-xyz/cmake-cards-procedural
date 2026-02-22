//i love my husband a lot, and his game is very cool i wish him good fortune and auspicious game vibes and i love him a lot again and the kitties love him too he is a good kitty dad and a good husband yay <3

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
    cardFrame_Action_Flow,
};

raylib::Texture2D const &GetTexture(GameTexture tex);

raylib::Texture2D const &GetTexture(CardBanner banner, CardType cardType);

raylib::Texture2D const &GetCardArtTexture(int cardId);
