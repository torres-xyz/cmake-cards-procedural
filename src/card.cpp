#include "card.hpp"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "csv.h"
#include "helper_functions.hpp"

std::vector<Card> GetCardDB()
{
    static std::vector<Card> cardDB{};
    if (cardDB.empty())
    {
        io::CSVReader<8> in("resources/csv/cards_db.csv");
        in.read_header(io::ignore_extra_column, "cardID", "Name", "Type", "Banner", "Body Text", "Body", "Mind", "Soul");
        int cardID{};
        std::string name{};
        std::string cardType{};
        std::string cardBanner{};
        std::string bodyText{};
        int body{};
        int mind{};
        int soul{};

        while (in.read_row(cardID, name, cardType, cardBanner, bodyText, body, mind, soul))
        {
            //TODO: verify that using emplace_back() like this makes sense and does avoid making a copy
            cardDB.emplace_back() = {
                .type = StringToCardType(cardType),
                .cardID = cardID,
                .name = name,
                .bodyText = bodyText,
                .banner = StringToBanner(cardBanner),
                .body = body,
                .mind = mind,
                .soul = soul
            };
        }
    }

    return cardDB;
}

Card GetCardFromDB(const int id)
{
    io::CSVReader<8> in("resources/csv/cards_db.csv");
    in.read_header(io::ignore_extra_column, "cardID", "Name", "Type", "Banner", "Body Text", "Body", "Mind", "Soul");
    int cardID{};
    std::string name{};
    std::string cardType{};
    std::string cardBanner{};
    std::string bodyText{};
    int body{};
    int mind{};
    int soul{};

    Card newCard{};
    while (in.read_row(cardID, name, cardType, cardBanner, bodyText, body, mind, soul))
    {
        if (cardID != id) continue;

        newCard.type = StringToCardType(cardType);
        newCard.cardID = cardID;
        newCard.name = name;
        newCard.bodyText = bodyText;
        newCard.banner = StringToBanner(cardBanner);
        newCard.body = body;
        newCard.mind = mind;
        newCard.soul = soul;
        newCard.uid = HelperFunctions::GetUID();

        break;
    }

    return newCard;
}

void PrintCard(const Card &card)
{
    const std::string cardPrint{
        std::format("Printing Card [Name: {0}, ID: {1}, Type: {2}, Banner: {3}, Body Text: {4}, Body: {5}, Mind: {6}, Soul: {7}]\n",
                    card.name,
                    std::to_string(static_cast<int>(card.cardID)),
                    CardTypeToString(card.type),
                    BannerToString(card.banner),
                    card.bodyText,
                    std::to_string(card.body),
                    std::to_string(card.mind),
                    std::to_string(card.soul))
    };

    std::cout << cardPrint;
}

CardBanner StringToBanner(const std::string_view sv)
{
    const std::unordered_map<std::string, CardBanner> stringToBannerMap
    {
        {"Form", CardBanner::form},
        {"Flow", CardBanner::flow},
        {"Strategy", CardBanner::strategy},
        {"Instinct", CardBanner::instinct},
        {"Hope", CardBanner::hope},
        {"Despair", CardBanner::despair},
    };

    assert(stringToBannerMap.contains(sv.data()) && "No card banner with such name");

    return stringToBannerMap.at(sv.data());
}

std::string BannerToString(const CardBanner banner)
{
    const std::unordered_map<CardBanner, std::string> stringToBannerMap
    {
        {CardBanner::invalid, "Invalid"},
        {CardBanner::form, "Form"},
        {CardBanner::flow, "Flow"},
        {CardBanner::strategy, "Strategy"},
        {CardBanner::instinct, "Instinct"},
        {CardBanner::hope, "Hope"},
        {CardBanner::despair, "Despair"},
    };

    assert(stringToBannerMap.contains(banner) && "No such Banner.");

    return stringToBannerMap.at(banner);
}

CardType StringToCardType(const std::string_view sv)
{
    const std::unordered_map<std::string, CardType> stringToCardTypeMap
    {
        {"Unit", CardType::unit},
        {"Action", CardType::action},
    };

    assert(stringToCardTypeMap.contains(sv.data()) && "No card type with such name");

    return stringToCardTypeMap.at(sv.data());
}

std::string CardTypeToString(const CardType cardType)
{
    const std::unordered_map<CardType, std::string> cardTypeToStringMap
    {
        {CardType::invalid, "Invalid"},
        {CardType::unit, "Unit"},
        {CardType::action, "Action"},
    };

    assert(cardTypeToStringMap.contains(cardType) && "No such card type.");

    return cardTypeToStringMap.at(cardType);
}

int StringToCardId(const std::string_view cardName)
{
    const std::vector<Card> cardDB = GetCardDB();

    for (const Card &card: cardDB)
    {
        if (card.name == cardName)
        {
            return card.cardID;
        }
    }

    return 0;
}
