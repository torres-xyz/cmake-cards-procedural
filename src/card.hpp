#pragma once
#include "constants.hpp"
#include "raylib-cpp.hpp"

enum class CardType
{
    invalid,
    unit,
    action
};

enum class CardID
{
    invalid = 0,
    cancerPagurus = 1,
    pierrotten = 2,
    bodyPlus = 15,
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
    raylib::Rectangle rect
    {
        0, 0,
        constants::cardWidth,
        constants::cardHeight
    };
    CardType type{CardType::invalid};
    bool faceUp{true};
    CardID cardID{};
    unsigned long int uid{};
    std::string name{};
    std::string bodyText{};
    CardBanner banner{CardBanner::invalid};
    int body{0};
    int mind{0};
    int soul{0};
};

void LoadCardLibrary();

Card GetCardFromDB(const CardID id);

void PrintCard(const Card &card);

CardBanner StringToBanner(const std::string_view sv);

std::string BannerToString(const CardBanner banner);

CardType StringToCardType(const std::string_view sv);

std::string CardTypeToString(const CardType cardType);
