#include "textures.hpp"

#include <map>

#include "card.hpp"

raylib::Texture2D const &GetTexture(const GameTexture tex)
{
    static const std::map<GameTexture, std::string> gameTextureToPathMap
    {
        {GameTexture::metal08, "resources/textures/metal_08.jpg"},
        {GameTexture::metal22, "resources/textures/metal_22.jpg"},
        {GameTexture::metal35, "resources/textures/metal_35.jpg"},
        {GameTexture::wall13, "resources/textures/wall_13.png"},
        {GameTexture::wood11, "resources/textures/wood_11.png"},
        {GameTexture::panel01, "resources/textures/panel_01.png"},
        {GameTexture::panel05, "resources/textures/panel_05.png"},
        {GameTexture::cardBack, "resources/textures/cards/card_back.png"},
        {GameTexture::paperCard, "resources/textures/cards/paper_card.png"},
        {GameTexture::rockCard, "resources/textures/cards/rock_card.png"},
        {GameTexture::scissorsCard, "resources/textures/cards/scissors_card.png"},
    };
    static std::map<GameTexture, raylib::Texture2D> gameTextureToTexture2DMap{};

    if (!gameTextureToTexture2DMap.contains(tex))
    {
        gameTextureToTexture2DMap.insert(
            {
                tex,
                raylib::Texture2D(gameTextureToPathMap.at(tex))
            });
    }

    return gameTextureToTexture2DMap.at(tex);
}

raylib::Texture2D const &GetTexture(const CardType type)
{
    return GetTexture(GetGameTextureFromCardType(type));
}

GameTexture GetGameTextureFromCardType(const CardType type)
{
    static const std::map<CardType, GameTexture> cardTypeToTextureMap
    {
        {CardType::invalid, GameTexture::invalid},
        {CardType::paper, GameTexture::paperCard},
        {CardType::rock, GameTexture::rockCard},
        {CardType::scissors, GameTexture::scissorsCard},
    };

    return cardTypeToTextureMap.at(type);
};
