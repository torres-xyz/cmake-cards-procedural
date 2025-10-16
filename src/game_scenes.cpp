#include "game_scenes.hpp"
#include <cassert>
#include <iostream>

#include "audio.hpp"
#include "constants.hpp"
#include "game_play_phases.hpp"
#include "player.hpp"
#include "textures.hpp"

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

void DrawCard(const Card &card)
{
    const raylib::Texture2D &cardTex = GetTexture(
        card.faceUp
            ? GetGameTextureFromCardType(card.type)
            : GameTexture::cardBack);

    const raylib::Rectangle cardTexSourceRect
    {
        0, 0,
        static_cast<float>(cardTex.GetWidth()),
        static_cast<float>(cardTex.GetHeight())
    };
    const raylib::Rectangle cardTextDestRect
    {
        card.pos.x,
        card.pos.y,
        constants::cardWidth,
        constants::cardHeight
    };

    cardTex.Draw(cardTexSourceRect, cardTextDestRect);
}

bool CheckCollisionPointCard(const raylib::Vector2 &point, const Card &card)
{
    return CheckCollisionPointRec
    (
        point,
        raylib::Rectangle
        {
            card.pos.x,
            card.pos.y,
            card.size.x,
            card.size.y
        }
    );
}

void RunPlayingScene(PlayingScene &playingScene, GameplayPhase &currentPhase, Player &player1, Player &player2, const int goingFirst, std::random_device &rd)
{
    const raylib::Vector2 mousePosition = GetMousePosition();
    static bool player1HasDrawnThisTurn;
    static raylib::Vector2 heldCardOffset{};
    int hoveredCardIndex{-1};

    if (currentPhase == GameplayPhase::uninitialized)
    {
        player1HasDrawnThisTurn = false;
        heldCardOffset = raylib::Vector2{0, 0};
        hoveredCardIndex = -1;
    }

    auto UpdateDeckButton{
        [&player1, currentPhase, &playingScene, mousePosition]()-> void {
            Button &playerDeckButton = playingScene.playerDeckButton;
            playerDeckButton.state = ButtonState::disabled;

            if (!player1.deck.empty() && (currentPhase == GameplayPhase::initialHandDraw || currentPhase == GameplayPhase::playerOneDrawing))
            {
                playerDeckButton.state = ButtonState::enabled;
                UpdateButtonState(playerDeckButton, mousePosition, IsMouseButtonDown(MOUSE_BUTTON_LEFT), IsMouseButtonReleased(MOUSE_BUTTON_LEFT));

                if (playerDeckButton.wasPressed)
                {
                    PlaySound(GameSound::buttonPress01);
                    DrawCardsFromDeckToHand(player1, 1);
                    player1HasDrawnThisTurn = true;
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

    //Update Music
    PlayMusic(playingScene.music);

    //Running the gameplay
    UpdateGameplayPhases(currentPhase, player1, player2, goingFirst, player1HasDrawnThisTurn, rd);

    //Scene Buttons
    UpdateDeckButton();

    //Handle Card Positioning

    //If we let go of the LMB
    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
    {
        //and we are placing the card on the play zone we select that card to be played
        if (currentPhase == GameplayPhase::playerOnePlaying &&
            player1.isHoldingACard &&
            player1.cardInPlay.type == CardType::invalid)
        {
            const Card &heldCard = player1.hand.at(static_cast<size_t>(player1.heldCardIndex));
            const Rectangle cardRec
            {
                heldCard.pos.x,
                heldCard.pos.y,
                heldCard.size.x,
                heldCard.size.y
            };

            if (CheckCollisionRecs(cardRec, constants::playfieldRec))
            {
                PutCardInPlay(player1);
            }
        }
        //we let go of the held card
        RemovePlayerHeldCard();
    }
    //Place cards in the Hand Zone or Update Held Card
    for (size_t i = 0; i < player1.hand.size(); ++i)
    {
        //Move the card we are holding with the mouse
        if (static_cast<int>(i) == player1.heldCardIndex)
        {
            player1.hand.at(i).pos = mousePosition - heldCardOffset;
            continue;
        }

        //If trying to click and hold a card
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
            CheckCollisionPointCard(mousePosition, player1.hand.at(i)) &&
            !player1.isHoldingACard)
        {
            SetPlayerHeldCard(static_cast<int>(i));
            heldCardOffset = mousePosition - player1.hand.at(i).pos;

            //Don't position this card in the hand zone.
            continue;
        }

        //Set cards in the hand slightly apart from each other.
        const int int_i = static_cast<int>(i);
        const raylib::Rectangle newCardRect
        {
            static_cast<float>(constants::handZonePosX + 2 * int_i + 1 + constants::cardWidth * int_i),
            static_cast<float>(constants::handZonePosY + 4),
            constants::cardWidth,
            constants::cardHeight
        };

        player1.hand.at(i).pos = raylib::Vector2{newCardRect.x, newCardRect.y};
        player1.hand.at(i).faceUp = true;

        //Handle hovering
        if (CheckCollisionPointCard(mousePosition, player1.hand.at(i)))
        {
            hoveredCardIndex = static_cast<int>(i);
        }
    }

    //  Draw
    GetTexture(playingScene.background).Draw();
    DrawButton(playingScene.playerDeckButton, GetTexture(playingScene.playerDeckButton.background));
    GetTexture(playingScene.playfield).Draw(constants::playfieldRec, constants::playfieldRec);

    //Draw Hovered Card or Held Card in the Preview Zone
    if (hoveredCardIndex != -1 || player1.isHoldingACard)
    {
        //Prefer showing our held card.
        int cardIndexToDisplay{};
        if (player1.isHoldingACard) cardIndexToDisplay = player1.heldCardIndex;
        else if (hoveredCardIndex != -1) cardIndexToDisplay = hoveredCardIndex;
        else return;

        const raylib::Texture2D &cardTex = GetTexture(player1.hand.at(static_cast<std::size_t>(cardIndexToDisplay)).type);

        const raylib::Rectangle cardTexSourceRect
        {
            0, 0,
            static_cast<float>(cardTex.GetWidth()),
            static_cast<float>(cardTex.GetHeight())
        };

        cardTex.Draw(cardTexSourceRect, constants::cardPreviewZoneRec);
    }
    else
    {
        //Draw the Preview Zone background
        GetTexture(playingScene.cardPreviewZoneTex).Draw(constants::cardPreviewZoneRec, constants::cardPreviewZoneRec);
    }

    //Draw Cards in the playfield
    if (player1.cardInPlay.type != CardType::invalid)
    {
        DrawCard(player1.cardInPlay);
    }
    if (player2.cardInPlay.type != CardType::invalid)
    {
        DrawCard(player2.cardInPlay);
    }

    //Draw Cards in Hand, ...
    for (size_t i = 0; i < player1.hand.size(); ++i)
    {
        if (player1.heldCardIndex == static_cast<int>(i)) continue;
        DrawCard(player1.hand.at(i));
    }
    //... with the one being held drawn last, on top.
    if (player1.isHoldingACard)
    {
        DrawCard(player1.hand.at(static_cast<std::size_t>(player1.heldCardIndex)));
    }
}

void RunGameOverScene(GameOverScene &gameOverScene, GameScene &currentScene, GameplayPhase &gameplayPhase)
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
}

