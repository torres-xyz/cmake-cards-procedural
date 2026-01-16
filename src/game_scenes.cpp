#include "game_scenes.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <format>

#include "audio.hpp"
#include "constants.hpp"
#include "game_play_phases.hpp"
#include "player.hpp"
#include "textures.hpp"
#include "helper_functions.hpp"
#include "fonts.hpp"
#include "game_rules.hpp"
#include "game_status.hpp"

struct GameStatus;

void RunStartingScene(StartingScene &startingScene, GameScene &currentScene)
{
    const raylib::Vector2 mousePosition = GetMousePosition();
    Button &startButton = startingScene.startButton;
    // Update
    // Update Buttons
    UpdateButtonState(startButton,
                      mousePosition,
                      IsMouseButtonDown(MOUSE_BUTTON_LEFT),
                      IsMouseButtonReleased(MOUSE_BUTTON_LEFT));
    if (startButton.wasPressed)
    {
        PlaySound(GameSound::buttonPress01);
        currentScene = GameScene::playing;
    }

    // Update Music
    PlayMusic(startingScene.music);

    //Draw
    GetTexture(startingScene.background).Draw();
    DrawButton(startButton, GetTexture(startButton.background));
}

void DrawTextInsideCard(const char *text, const raylib::Rectangle &destRect, const float x, const float y,
                        const float width, const float height, const raylib::Vector2 margins,
                        const float fontMultiplier, const bool drawGrayBox)
{
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
}

void DrawCardAdvanced(const Card &card, const raylib::Rectangle destinationRect)
{
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
    DrawTextInsideCard(card.name.c_str(), destinationRect, 49, 45, 523, 56, margins, 0.05f, false);
    DrawTextInsideCard(card.bodyText.c_str(), destinationRect, 49, 532, 626, 337, margins, 0.06f, false);
    DrawTextInsideCard(std::to_string(card.body).c_str(), destinationRect, 65, 894, 161, 77, margins, 0.07f, false);
    DrawTextInsideCard(std::to_string(card.mind).c_str(), destinationRect, 280, 894, 161, 77, margins, 0.07f, false);
    DrawTextInsideCard(std::to_string(card.soul).c_str(), destinationRect, 494, 894, 161, 77, margins, 0.07f, false);
}

void RunPlayingScene(PlayingScene &playingScene, GameplayPhase &currentPhase, GameStatus &gameStatus, Player &player1, Player &player2, const GameRules &gameRules, std::random_device &rd)
{
    // UPDATE ------------------------------------------------------------------

    UpdateGameplayPhases(playingScene, currentPhase, gameStatus, player1, player2, gameRules, rd);

    //Update Music
    PlayMusic(playingScene.music);

    // DRAW ---------------------------------------------------------------------
    GetTexture(playingScene.background).Draw();
    DrawButton(playingScene.playerDeckButton, GetTexture(playingScene.playerDeckButton.background));
    DrawButton(playingScene.endTurnButton, GetTexture(playingScene.endTurnButton.background));
    GetTexture(playingScene.playfield).Draw(
        Rectangle{
            0,
            0,
            constants::playfieldRec.width,
            constants::playfieldRec.height
        },
        constants::playfieldRec);
    // Draw stack stats total
    // Draw stack stats total - Rectangle
    const raylib::Rectangle statsTotalRec
    {
        constants::playfieldRec.x,
        constants::playfieldRec.y + constants::playfieldRec.height - 50,
        constants::playfieldRec.width,
        50
    };
    const raylib::Color statsTotalRecColor{10, 10, 10, 200};
    DrawRectangleRec(statsTotalRec, statsTotalRecColor);

    // Draw stack stats total - Player 1 Stats
    const std::string player1TotalCardStats
    {
        std::format("Total Stats \nB: {0} | M: {1} | S: {2}",
                    std::to_string(GetCardStackTotalBody(player1.cardsInPlayStack)),
                    std::to_string(GetCardStackTotalMind(player1.cardsInPlayStack)),
                    std::to_string(GetCardStackTotalSoul(player1.cardsInPlayStack)))
    };

    const raylib::Rectangle player1StatsRec
    {
        constants::playfieldRec.x,
        constants::playfieldRec.y + constants::playfieldRec.height - 50,
        constants::playfieldRec.width * 0.5,
        50
    };
    HelperFunctions::DrawTextBoxed
    (
        GetFont(GameFont::aobashiOne),
        player1TotalCardStats.c_str(),
        player1StatsRec,
        20,
        0.1f,
        0.5f,
        true,
        WHITE
    );
    // Draw stack stats total - Player 2 Stats
    const std::string player2TotalCardStats{
        std::format("Total Stats \nB: {0} | M: {1} | S: {2}",
                    std::to_string(GetCardStackTotalBody(player2.cardsInPlayStack)),
                    std::to_string(GetCardStackTotalMind(player2.cardsInPlayStack)),
                    std::to_string(GetCardStackTotalSoul(player2.cardsInPlayStack)))
    };
    const raylib::Rectangle player2StatsRec
    {
        constants::playfieldRec.x + constants::playfieldRec.width * 0.5,
        constants::playfieldRec.y + constants::playfieldRec.height - 50,
        constants::playfieldRec.width * 0.5,
        50
    };
    HelperFunctions::DrawTextBoxed
    (
        GetFont(GameFont::aobashiOne),
        player2TotalCardStats.c_str(),
        player2StatsRec,
        20,
        0.1f,
        0.5f,
        true,
        WHITE
    );

    //Join all cards in the order we want to draw them in
    //and draw them in a single for loop, skipping over the hovered card, which is drawn last.
    std::vector<Card> allCards{};
    //// Player 2 Hand
    allCards.insert(std::end(allCards), std::begin(player2.hand), std::end(player2.hand));
    //// Cards in player 1 hand
    allCards.insert(std::end(allCards), std::begin(player1.hand), std::end(player1.hand));
    //// Cards in the playfield
    const std::size_t playfieldCardsIndexOffset{allCards.size()};
    allCards.insert(std::end(allCards), std::begin(player1.cardsInPlayStack), std::end(player1.cardsInPlayStack));
    allCards.insert(std::end(allCards), std::begin(player2.cardsInPlayStack), std::end(player2.cardsInPlayStack));


    if (allCards.empty()) return;
    std::size_t hoveredCardIndex{};
    for (std::size_t i = 0; i < allCards.size(); ++i)
    {
        if (player1.isHoveringOverACard && player1.hoveredCardUid == allCards.at(i).uid)
        {
            hoveredCardIndex = i;
            continue;
        }

        DrawCardAdvanced(allCards.at(i), allCards.at(i).rect);
    }
    //Draw hovered card last
    if (player1.isHoveringOverACard)
    {
        if (hoveredCardIndex < playfieldCardsIndexOffset)
        {
            //Expand the cards in the hand in place, but move it up a bit.
            const raylib::Rectangle expandedCardRect
            {
                std::max(10.0f, allCards.at(hoveredCardIndex).rect.x - (constants::zoomedCardWidth - allCards.at(hoveredCardIndex).rect.width) * 0.5f),
                allCards.at(hoveredCardIndex).rect.y - allCards.at(hoveredCardIndex).rect.height * 1.3f,
                constants::zoomedCardWidth,
                constants::zoomedCardHeight
            };
            DrawCardAdvanced(allCards.at(hoveredCardIndex), expandedCardRect);
        }
        else
        {
            //Expand the cards in the play field in place
            const raylib::Rectangle expandedCardInFieldRect
            {
                allCards.at(hoveredCardIndex).rect.x - (constants::zoomedCardWidth - allCards.at(hoveredCardIndex).rect.width) * 0.5f,
                allCards.at(hoveredCardIndex).rect.y,
                constants::zoomedCardWidth,
                constants::zoomedCardHeight
            };
            DrawCardAdvanced(allCards.at(hoveredCardIndex), expandedCardInFieldRect);
        }
    }
    if (currentPhase == GameplayPhase::endPhase)
    {
        //Render a semi-transparent black overlay on the scene
        const raylib::Rectangle endPhaseOverlayRec{0, 0, constants::screenWidth, constants::screenHeight};
        const raylib::Color transparentBlack{0, 0, 0, 180};
        endPhaseOverlayRec.Draw(transparentBlack);

        const Button &nextRoundButton = playingScene.nextRoundButton;
        DrawButton(nextRoundButton, GetTexture(nextRoundButton.background));

        constexpr float rectLength{200};
        const raylib::Rectangle message
        {
            constants::screenWidth / 2.0 - rectLength / 2.0,
            10,
            rectLength,
            200
        };

        assert(!gameStatus.roundWinnerHistory.empty() && "gameStatus.roundWinnerHistory is empty.");

        if (gameStatus.roundWinnerHistory.back() == 0)
        {
            HelperFunctions::DrawTextCenteredInRec("Round ended in a tie.", 20, RAYWHITE, message);
        }
        if (gameStatus.roundWinnerHistory.back() == 1)
        {
            HelperFunctions::DrawTextCenteredInRec("Player 1 wins this round.", 20, RAYWHITE, message);
        }
        else if (gameStatus.roundWinnerHistory.back() == 2)
        {
            HelperFunctions::DrawTextCenteredInRec("Player 2 wins this round.", 20, RAYWHITE, message);
        }
    }
}

void RunGameOverScene(GameOverScene &gameOverScene, GameScene &currentScene, GameplayPhase &gameplayPhase,
                      const Player &player1, const Player &player2)
{
    const raylib::Vector2 mousePosition = GetMousePosition();

    Button &playAgainButton = gameOverScene.playAgainButton;
    // Update
    // Update Buttons
    UpdateButtonState(playAgainButton,
                      mousePosition,
                      IsMouseButtonDown(MOUSE_BUTTON_LEFT),
                      IsMouseButtonReleased(MOUSE_BUTTON_LEFT));
    if (playAgainButton.wasPressed)
    {
        PlaySound(GameSound::buttonPress01);

        gameplayPhase = GameplayPhase::uninitialized;
        currentScene = GameScene::playing;
    }

    // Update Music
    PlayMusic(gameOverScene.music);

    //Draw
    GetTexture(gameOverScene.background).Draw();
    DrawButton(playAgainButton, GetTexture(playAgainButton.background));


    constexpr float rectLength{200};
    const raylib::Rectangle winMessageRect
    {
        constants::screenWidth / 2.0 - rectLength / 2.0,
        10,
        rectLength,
        100
    };
    if (player1.score > player2.score)
    {
        HelperFunctions::DrawTextCenteredInRec("Player 1 Wins!", 20, BLACK, winMessageRect);
    }
    else if (player2.score > player1.score)
    {
        HelperFunctions::DrawTextCenteredInRec("Player 2 Wins!", 20, BLACK, winMessageRect);
    }
    else
    {
        HelperFunctions::DrawTextCenteredInRec("It's a Draw!", 20, BLACK, winMessageRect);
    }
}

void RunPrototypingScene(const PrototypingScene &scene)
{
    static Card advancedCardProt
    {
        .rect = {
            constants::cardWidth,
            constants::cardHeight,
            100,
            100
        },
        .type = CardType::unit,
        .faceUp = true,
        .cardID = CardID::cancerPagurus,
        .name = "Lorem Ipsum",
        .bodyText = "Id aspernatur consequuntur eos ut quia vero. Voluptas "
        "beatae ut temporibus consectetur eveniet placeat adipisci. "
        "Dignissimos aut et recusandae voluptates harum. Enim non et "
        "facilis. Nemo reiciendis dolores dolores illum omnis "
        "voluptatem.",
        .banner = CardBanner::form,
        .body = 3000,
        .mind = 2000,
        .soul = 1000
    };

    advancedCardProt.rect.SetSize(
        GetMousePosition().x - advancedCardProt.rect.x,
        (GetMousePosition().x - advancedCardProt.rect.x) * (1 / constants::cardAspectRatio)
    );

    //Draw
    GetTexture(scene.background).Draw();
    DrawCardAdvanced(advancedCardProt, advancedCardProt.rect);
}

