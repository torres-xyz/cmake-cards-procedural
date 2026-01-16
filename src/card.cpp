#include "card.hpp"

#include <iostream>
#include <unordered_map>

#include "csv.h"

void LoadCardLibrary()
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
        Card newCard
        {
            .type = static_cast<CardType>(cardType),
            .cardID = static_cast<CardID>(cardID),
            .name = name,
            .bodyText = bodyText,
            .banner = StringToBanner(cardBanner),
            .body = body,
            .mind = mind,
            .soul = soul
        };
        PrintCard(newCard);
    }
}

void PrintCard(const Card &card)
{
    const std::string cardPrint{
        std::format("Printing Card: Name: {0}, ID: {1}, Type: {2}, Banner: {3}, Body Text: {4}, Body: {5}, Mind {6}, Soul{7}",
                    card.name,
                    std::to_string(static_cast<int>(card.cardID)),
                    std::to_string(static_cast<int>(card.type)),
                    std::to_string(static_cast<int>(card.banner)),
                    card.bodyText,
                    std::to_string(card.body),
                    std::to_string(card.mind),
                    std::to_string(card.soul))
    };

    std::cout << cardPrint;
}

CardBanner StringToBanner(const std::string_view sv)
{
    const std::pmr::unordered_map<std::string, CardBanner> stringToBannerMap
    {
        {"Form", CardBanner::form},
        {"Flow", CardBanner::flow},
        {"Strategy", CardBanner::strategy},
        {"Instinct", CardBanner::instinct},
        {"Hope", CardBanner::hope},
        {"Despair", CardBanner::despair},
    };

    if (!stringToBannerMap.contains(sv.data())) return CardBanner::invalid;

    return stringToBannerMap.at(sv.data());
}
