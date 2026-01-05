#include "game_scenes.hpp"
#include <cassert>
#include <iostream>

#include "audio.hpp"
#include "constants.hpp"
#include "game_play_phases.hpp"
#include "player.hpp"
#include "textures.hpp"
#include "helper_functions.hpp"
#include "fonts.hpp"
#include "game_rules.hpp"

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
    const raylib::Texture2D &cardArtTex = GetCardArtTexture(card.id);
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

void RunPlayingScene(PlayingScene &playingScene, GameplayPhase &currentPhase, Player &player1, Player &player2, const int goingFirst, std::random_device &rd)
{
    const raylib::Vector2 mousePosition = GetMousePosition();
    static raylib::Vector2 heldCardOffset{};
    int hoveredCardIndex{-1};

    // UPDATE ------------------------------------------------------------------

    NEW_UpdateGameplayPhases(playingScene, currentPhase, player1, player2, goingFirst, rd);

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


    //Draw Player 2 Hand
    for (std::size_t i = 0; i < player2.hand.size(); ++i)
    {
        const auto index = static_cast<float>(i);
        //Set the cards in the hand slightly apart from each other.
        const raylib::Vector2 cardPos{
            constants::player2HandZone.x + 2 * (index + 1) + constants::cardWidth * index,
            constants::player2HandZone.y + 4,
        };

        player2.hand.at(i).rect.SetPosition(cardPos);
        player2.hand.at(i).faceUp = false;
        DrawCardAdvanced(player2.hand.at(i), player2.hand.at(i).rect);
    }

    //Draw Cards in the playfield
    if (!player1.cardsInPlayStack.empty())
    {
        DrawCardAdvanced(player1.cardsInPlayStack.at(0), constants::playerOnePlayfieldCardZoneRect);
    }
    if (!player2.cardsInPlayStack.empty())
    {
        DrawCardAdvanced(player2.cardsInPlayStack.at(0), constants::playerTwoPlayfieldCardZoneRect);
    }


    //Draw Cards in Player 1 Hand, ...
    for (size_t i = 0; i < player1.hand.size(); ++i)
    {
        if (player1.heldCardIndex == static_cast<int>(i)) continue;
        if (hoveredCardIndex == static_cast<int>(i)) continue;
        // DrawCard(player1.hand.at(i));
        DrawCardAdvanced(player1.hand.at(i), player1.hand.at(i).rect);
    }

    return;

    // if (currentPhase == GameplayPhase::uninitialized)
    // {
    //     heldCardOffset = raylib::Vector2{0, 0};
    //     hoveredCardIndex = -1;
    // }

    auto IsPlayerOnePlaying{
        [currentPhase]() -> bool {
            if (currentPhase == GameplayPhase::playerOneFirstTurn ||
                currentPhase == GameplayPhase::playerOnePlayingAndOpponentPassed ||
                currentPhase == GameplayPhase::playerOnePlayingAndOpponentPlayed)
            {
                return true;
            }
            return false;
        }
    };


    auto UpdateEndTurnButton{
        [&player1, &playingScene, mousePosition, IsPlayerOnePlaying]()-> void {
            Button &endTurnButton = playingScene.endTurnButton;
            endTurnButton.state = ButtonState::disabled;

            if (IsPlayerOnePlaying() && !player1.cardsInPlayStack.empty())
            {
                endTurnButton.state = ButtonState::enabled;
                UpdateButtonState(endTurnButton, mousePosition, IsMouseButtonDown(MOUSE_BUTTON_LEFT), IsMouseButtonReleased(MOUSE_BUTTON_LEFT));

                if (endTurnButton.wasPressed)
                {
                    PlaySound(GameSound::buttonPress01);
                    player1.hasEndedTheTurn = true;
                }
            }
        }
    };


    auto RemovePlayerHeldCard{
        [&player1]()-> void {
            player1.heldCardIndex = -1;
            player1.isHoldingACard = false;
        }
    };
    auto SetPlayerHeldCard{
        [&player1](const int index)-> void {
            assert(index >= 0 &&
                "Trying to set a Held Card index that is negative.");
            assert(index < static_cast<int>( player1.hand.size()) &&
                "Trying to set a Held Card index that is outside of the players hand bounds.");
            player1.heldCardIndex = index;
            player1.isHoldingACard = true;
        }

    };

    auto TryPlayingCard{
        [IsPlayerOnePlaying, &player1, currentPhase, RemovePlayerHeldCard]()-> void {
            //If Player 1 tries to play a card,
            //and we are placing the card on the play zone,
            //we select that card to be played
            if (IsPlayerOnePlaying() && player1.isHoldingACard)
            {
                const Card &heldCard = player1.hand.at(static_cast<size_t>(player1.heldCardIndex));

                if (CheckCollisionRecs(heldCard.rect, constants::playfieldRec) &&
                    CanCardBePlayedByPlayer(heldCard, player1, currentPhase))
                {
                    PutCardInPlay(player1);
                }
            }
            RemovePlayerHeldCard(); //whether we can play it or not, we always let go of the card from the hand.
        }
    };


    //Running the gameplay
    UpdateGameplayPhases(currentPhase, player1, player2, goingFirst, rd);

    //Scene Buttons
    // UpdateDeckButton();
    UpdateEndTurnButton();

    //Handle Card Positioning
    constexpr int cardWidth{constants::cardWidth * 2};
    constexpr int cardHeight{constants::cardHeight * 2};

    //If we let go of the LMB
    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
    {
        TryPlayingCard();
    }
    //Place cards in the Hand Zone or Update Held Card
    for (size_t i = 0; i < player1.hand.size(); ++i)
    {
        //Move the card we are holding with the mouse
        if (static_cast<int>(i) == player1.heldCardIndex)
        {
            player1.hand.at(i).rect.SetPosition(mousePosition - heldCardOffset);
            player1.hand.at(i).rect.SetSize(cardWidth, cardHeight);
            continue;
        }

        //If trying to click and hold a card
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
            HelperFunctions::CheckCollisionPointCard(mousePosition, player1.hand.at(i)) &&
            !player1.isHoldingACard)
        {
            SetPlayerHeldCard(static_cast<int>(i));
            heldCardOffset = mousePosition - player1.hand.at(i).rect.GetPosition();

            //Don't position this card in the hand zone.
            continue;
        }

        //Set cards in the hand slightly apart from each other.
        const int int_i = static_cast<int>(i);
        const raylib::Rectangle newCardRect
        {
            static_cast<float>(constants::handZonePosX + 2 * int_i + 1 + cardWidth * int_i),
            static_cast<float>(constants::handZonePosY + 4),
            cardWidth,
            cardHeight
        };

        player1.hand.at(i).rect = newCardRect;
        player1.hand.at(i).faceUp = true;

        //Handle hovering
        if (HelperFunctions::CheckCollisionPointCard(mousePosition, player1.hand.at(i)))
        {
            hoveredCardIndex = static_cast<int>(i);
        }
    }

    //Draw ---------------------------------------------------------------------
    GetTexture(playingScene.background).Draw();

    //Draw Hovered Card expanded
    if (hoveredCardIndex != -1)
    {
        const Card &hoveredCard{player1.hand.at(static_cast<std::size_t>(hoveredCardIndex))};
        if (player1.isHoldingACard)
        {
            DrawCardAdvanced(hoveredCard, hoveredCard.rect);
        }
        else
        {
            //Expand the card in place
            raylib::Rectangle expandedCardRect
            {
                hoveredCard.rect.x,
                hoveredCard.rect.y - hoveredCard.rect.height * 1.5f,
                hoveredCard.rect.width * 2,
                hoveredCard.rect.height * 2
            };
            DrawCardAdvanced(hoveredCard, expandedCardRect);
        }
    }
    //... with the one being held drawn last, on top.
    if (player1.isHoldingACard)
    {
        const Card &heldCard{player1.hand.at(static_cast<std::size_t>(player1.heldCardIndex))};
        DrawCardAdvanced(heldCard, heldCard.rect);
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
    // DrawRectangleRec(winMessageRect, WHITE);
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

void RunPrototypingScene(PrototypingScene &prototypingScene)
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
        .id = CardID::firstCard,
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

    // advancedCardProt.size = raylib::Vector2{
    //     GetMousePosition().x - advancedCardProt.pos.x,
    //     (GetMousePosition().x - advancedCardProt.pos.x) * (1 / constants::cardAspectRatio),
    // };

    // Card advancedCardProt_x2 = advancedCardProt;
    // advancedCardProt_x2.size = raylib::Vector2{constants::cardWidth * 2, constants::cardHeight * 2};
    // advancedCardProt_x2.pos.x = 200;
    //
    // Card advancedCardProt_x3 = advancedCardProt;
    // advancedCardProt_x3.size = raylib::Vector2{constants::cardWidth * 3, constants::cardHeight * 3};
    // advancedCardProt_x3.pos.x = 400;
    //
    // Card advancedCardProt_x4 = advancedCardProt;
    // advancedCardProt_x4.size = raylib::Vector2{constants::cardWidth * 4, constants::cardHeight * 4};
    // advancedCardProt_x4.pos.x = 700;

    //Draw
    GetTexture(prototypingScene.background).Draw();
    // DrawCardAdvanced(advancedCardProt_x2);
    // DrawCardAdvanced(advancedCardProt_x3);
    // DrawCardAdvanced(advancedCardProt_x4);
}

