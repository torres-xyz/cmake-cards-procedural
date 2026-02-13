#pragma once
#include "constants.hpp"
#include "raylib-cpp.hpp"

enum class CardType
{
    invalid,
    unit,
    action
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

struct CardStats
{
    int body{0};
    int mind{0};
    int soul{0};
};

struct Card
{
    raylib::Rectangle rect
    {
        0, 0,
        constants::cardWidth,
        constants::cardHeight
    };
    CardType type{CardType::invalid};
    bool faceUp{true};
    int cardID{};
    unsigned long int uid{};
    std::string name{};
    std::string bodyText{};
    CardBanner banner{CardBanner::invalid};
    CardStats stats
    {
        .body = 0,
        .mind = 0,
        .soul = 0
    };
};

void RenderCard(const Card &card, raylib::Rectangle destinationRect, bool highlighted = false);

const std::vector<Card> &GetCardDB();

Card GetCardFromDB(int id);

Card GetCardFromDB(std::string_view cardName);

void PrintCard(const Card &card);

constexpr CardBanner StringToBanner(std::string_view sv);

constexpr std::string BannerToString(CardBanner banner);

constexpr CardType StringToCardType(std::string_view sv);

constexpr std::string CardTypeToString(CardType cardType);

int StringToCardId(std::string_view cardName);
