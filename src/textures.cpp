#include "textures.hpp"
#include <unordered_map>
#include "card.hpp"

raylib::Texture2D const &GetTexture(const GameTexture tex)
{
    static const std::unordered_map<GameTexture, std::string> gameTextureToPathMap
    {
        {GameTexture::invalid, "resources/textures/invalid_tex.png"},
        {GameTexture::metal08, "resources/textures/metal_08.jpg"},
        {GameTexture::metal22, "resources/textures/metal_22.jpg"},
        {GameTexture::metal35, "resources/textures/metal_35.jpg"},
        {GameTexture::wall13, "resources/textures/wall_13.png"},
        {GameTexture::wood11, "resources/textures/wood_11.png"},
        {GameTexture::panel01, "resources/textures/panel_01.png"},
        {GameTexture::panel05, "resources/textures/panel_05.png"},
        {GameTexture::cardBack, "resources/textures/cards/card_back.png"},
        {GameTexture::cardFrame_Unit_Form, "resources/textures/card-frames/body_form_min.png"},
        {GameTexture::cardFrame_Unit_Flow, "resources/textures/card-frames/body_flow_min.png"},
        {GameTexture::cardFrame_Unit_Strategy, "resources/textures/card-frames/mind_strategy_min.png"},
        {GameTexture::cardFrame_Unit_Instinct, "resources/textures/card-frames/mind_instinct_min.png"},
        {GameTexture::cardFrame_Unit_Hope, "resources/textures/card-frames/soul_hope_min.png"},
        {GameTexture::cardFrame_Unit_Despair, "resources/textures/card-frames/soul_despair_min.png"},
        {GameTexture::cardFrame_Action_Form, "resources/textures/card-frames/body_form_action_min.png"},
    };
    static std::unordered_map<GameTexture, raylib::Texture2D> gameTextureToTexture2DMap{};

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

raylib::Texture2D const &GetTexture(const CardBanner banner, const CardType cardType)
{
    static const std::unordered_map<CardBanner, GameTexture> cardBannerToUnitCardFrameTextureMap
    {
        {CardBanner::invalid, GameTexture::invalid},
        {CardBanner::despair, GameTexture::cardFrame_Unit_Despair},
        {CardBanner::flow, GameTexture::cardFrame_Unit_Flow},
        {CardBanner::form, GameTexture::cardFrame_Unit_Form},
        {CardBanner::hope, GameTexture::cardFrame_Unit_Hope},
        {CardBanner::instinct, GameTexture::cardFrame_Unit_Instinct},
        {CardBanner::strategy, GameTexture::cardFrame_Unit_Strategy},
    };
    static const std::unordered_map<CardBanner, GameTexture> cardBannerToActionCardFrameTextureMap
    {
        {CardBanner::invalid, GameTexture::invalid},
        {CardBanner::form, GameTexture::cardFrame_Action_Form},
    };

    if (cardType == CardType::unit)
    {
        if (cardBannerToUnitCardFrameTextureMap.contains(banner))
        {
            return GetTexture(cardBannerToUnitCardFrameTextureMap.at(banner));
        }
    }
    if (cardType == CardType::action)
    {
        if (cardBannerToActionCardFrameTextureMap.contains(banner))
        {
            return GetTexture(cardBannerToActionCardFrameTextureMap.at(banner));
        }
    }
    return GetTexture(GameTexture::invalid);
}

raylib::Texture2D const &GetCardArtTexture(const CardID cardId)
{
    static const std::unordered_map<CardID, std::string> cardIDToArtTexturePathMap
    {
        {CardID::invalid, "resources/textures/invalid_tex.png"},
        {CardID::firstCard, "resources/textures/card-art/first_card_art_min.png"},
        {CardID::firstCardAction, "resources/textures/card-art/first_card_art_action_min.png"},
    };
    static std::unordered_map<CardID, raylib::Texture2D> cardIDToTexture2DMap{};

    if (!cardIDToTexture2DMap.contains(cardId))
    {
        cardIDToTexture2DMap.insert(
            {
                cardId,
                raylib::Texture2D(cardIDToArtTexturePathMap.at(cardId))
            });
    }

    return cardIDToTexture2DMap.at(cardId);
}
