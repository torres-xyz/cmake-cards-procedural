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
    int body{0};
    int mind{0};
    int soul{0};
};

void RenderCard(const Card &card, raylib::Rectangle destinationRect);

std::vector<Card> GetCardDB();

Card GetCardFromDB(int id);

Card GetCardFromDB(std::string_view cardName);

void PrintCard(const Card &card);

CardBanner StringToBanner(std::string_view sv);

std::string BannerToString(CardBanner banner);

CardType StringToCardType(std::string_view sv);

std::string CardTypeToString(CardType cardType);

int StringToCardId(std::string_view cardName);
