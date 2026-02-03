#include "card.hpp"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "csv.h"
#include "fonts.hpp"
#include "helper_functions.hpp"
#include "textures.hpp"

void RenderCard(const Card &card, const raylib::Rectangle destinationRect)
{
    auto RenderTextInsideCard = [](const char *text, const raylib::Rectangle &destRect, const float x, const float y,
                                   const float width, const float height, const raylib::Vector2 margins,
                                   const float fontMultiplier, const bool drawGrayBox)-> void {
        //Coords
        const float textPosX{(x + margins.x) / constants::cardTextureWidth};
        const float textPosY{(y + margins.y) / constants::cardTextureHeight};
        const float textWidth{(width - margins.x * 2) / constants::cardTextureWidth};
        const float textHeight{(height - margins.y * 2) / constants::cardTextureHeight};
        const raylib::Rectangle textBoxRect
        {
            destRect.x + destRect.width * textPosX,
            destRect.y + destRect.height * textPosY,
            destRect.width * textWidth,
            destRect.height * textHeight
        };
        if (drawGrayBox)
        {
            DrawRectangleRec(textBoxRect, GRAY);
        }
        HelperFunctions::DrawTextBoxed
        (
            GetFont(GameFont::aobashiOne),
            text,
            textBoxRect,
            destRect.height * fontMultiplier,
            0.1f,
            0.5f,
            true,
            WHITE
        );
    };

    if (card.faceUp == false)
    {
        const raylib::Texture2D &cardBackTex = GetTexture(GameTexture::cardBack);

        const raylib::Rectangle cardBackTexSourceRect
        {
            0, 0,
            static_cast<float>(cardBackTex.GetWidth()),
            static_cast<float>(cardBackTex.GetHeight())
        };
        const raylib::Rectangle cardBackTextDestRect
        {
            destinationRect.x,
            destinationRect.y,
            destinationRect.width,
            destinationRect.height
        };

        cardBackTex.Draw(cardBackTexSourceRect, cardBackTextDestRect);
        return;
    }

    //Card Art
    const raylib::Texture2D &cardArtTex = GetCardArtTexture(card.cardID);
    const raylib::Rectangle cardArtTexSourceRect
    {
        0, 0,
        static_cast<float>(cardArtTex.GetWidth()),
        static_cast<float>(cardArtTex.GetHeight())
    };

    // constexpr float artMargin = 2.0;
    constexpr float artPosX{46.0f / constants::cardTextureWidth};
    constexpr float artPosY{123.0f / constants::cardTextureHeight};
    constexpr float artWidth{static_cast<float>(constants::cardArtTextureWidth) / constants::cardTextureWidth};
    constexpr float artHeight{static_cast<float>(constants::cardArtTextureHeight) / constants::cardTextureHeight};
    const raylib::Rectangle cardArtTexDestRect
    {
        destinationRect.x + destinationRect.width * artPosX,
        destinationRect.y + destinationRect.height * artPosY,
        destinationRect.width * artWidth,
        destinationRect.height * artHeight
    };
    cardArtTex.Draw(cardArtTexSourceRect, cardArtTexDestRect);

    //Card Frame
    const raylib::Texture2D &cardFrameTex = GetTexture(card.banner, card.type);
    const raylib::Rectangle cardFrameTexSourceRect
    {
        0, 0,
        static_cast<float>(cardFrameTex.GetWidth()),
        static_cast<float>(cardFrameTex.GetHeight())
    };
    const raylib::Rectangle cardFrameTexDestRect
    {
        destinationRect.x,
        destinationRect.y,
        destinationRect.width,
        destinationRect.height
    };
    cardFrameTex.Draw(cardFrameTexSourceRect, cardFrameTexDestRect);

    //if a card is being drawn at 1x size, don't render the text
    // if (destinationRect.width <= constants::cardWidth) return;

    //Hard coded numbers based on the texture pixel positions of these elements.
    const raylib::Vector2 margins{4, 0};
    RenderTextInsideCard(card.name.c_str(), destinationRect, 49, 45, 523, 56, margins, 0.05f, false);
    RenderTextInsideCard(card.bodyText.c_str(), destinationRect, 49, 532, 626, 337, margins, 0.06f, false);
    RenderTextInsideCard(std::to_string(card.body).c_str(), destinationRect, 65, 894, 161, 77, margins, 0.07f, false);
    RenderTextInsideCard(std::to_string(card.mind).c_str(), destinationRect, 280, 894, 161, 77, margins, 0.07f, false);
    RenderTextInsideCard(std::to_string(card.soul).c_str(), destinationRect, 494, 894, 161, 77, margins, 0.07f, false);
}

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
            cardDB.emplace_back() = {
                .type = StringToCardType(cardType),
                .cardID = cardID,
                .uid = HelperFunctions::GetUID(), //always generate a new UID when creating a new card
                .name = name,
                .bodyText = bodyText,
                .banner = StringToBanner(cardBanner),
                .body = body,
                .mind = mind,
                .soul = soul,
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
        newCard.uid = HelperFunctions::GetUID(); //always generate a new UID when creating a new card

        break;
    }

    return newCard;
}

Card GetCardFromDB(const std::string_view cardName)
{
    return GetCardFromDB(StringToCardId(cardName));
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
