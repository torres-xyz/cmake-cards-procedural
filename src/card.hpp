#pragma once
#include "constants.hpp"
#include "raylib-cpp.hpp"

enum class CardType
{
    invalid,
    rock,
    paper,
    scissors,
    prototypeCard
};

enum class CardBanner
{
    invalid,
    form,     // Body - Order
    flow,     // Body - Chaos
    strategy, // Mind - Order
    instinct, // Mind - Chaos
    hope,     // Soul - Order
    despair   // Soul - Chaos
};

struct Card
{
    raylib::Vector2 size
    {
        constants::cardWidth,
        constants::cardHeight
    };
    raylib::Vector2 pos{};
    CardType type{CardType::invalid};
    bool faceUp{true};

    // Advanced stats
    std::string name{};
    std::string bodyText{};
    CardBanner banner {CardBanner::invalid};
    int body {0};
    int mind {0};
    int soul {0};
};

