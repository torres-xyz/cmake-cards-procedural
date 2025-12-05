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

enum class CardID
{
    invalid,
    firstCard
};

enum class CardBanner
{
    invalid,
    form, // Body - Order
    flow, // Body - Chaos
    strategy, // Mind - Order
    instinct, // Mind - Chaos
    hope, // Soul - Order
    despair // Soul - Chaos
};

struct Card
{
    //TODO: convert size and pos into card Rectangle
    raylib::Rectangle rect
    {
        0, 0,
        constants::cardWidth,
        constants::cardHeight

    };
    CardType type{CardType::invalid};
    bool faceUp{true};

    // Advanced stats
    CardID id{};
    std::string name{};
    std::string bodyText{};
    CardBanner banner{CardBanner::invalid};
    int body{0};
    int mind{0};
    int soul{0};
};

