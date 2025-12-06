#pragma once
#include "raylib-cpp.hpp"

enum class GameFont
{
    invalid,
    aobashiOne,
};

raylib::Font const &GetFont(GameFont gameFont);
