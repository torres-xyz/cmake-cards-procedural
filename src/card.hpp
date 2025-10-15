#pragma once
#include "raylib-cpp.hpp"

enum class CardType
{
    invalid,
    rock,
    paper,
    scissors
};

struct Card
{
    raylib::Vector2 size{};
    raylib::Vector2 pos{};
    CardType type{CardType::invalid};
    bool faceUp{true};
};
