#include "game_scenes.hpp"

#include "audio.hpp"
#include "constants.hpp"
#include "game_play_phases.hpp"
#include "player.hpp"
#include "textures.hpp"

void RunStartingScene(StartingScene &startingScene,
                      GameScene &currentScene, const Vector2 &mousePosition)
{
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

void DrawCardPreview(const Card &card)
{
    const raylib::Texture2D &cardTex = GetTexture(card.type);

    const raylib::Rectangle cardTexSourceRect
    {
        0, 0,
        static_cast<float>(cardTex.GetWidth()),
        static_cast<float>(cardTex.GetHeight())
    };

    cardTex.Draw(cardTexSourceRect, constants::cardPreviewZoneRec);
}


void RunPlayingScene(PlayingScene &playingScene, const raylib::Vector2 &mousePosition,
                     GameplayPhase &currentPhase, Player &player1, Player &player2, const int goingFirst)
{
    static bool holdingCard{false};
    //Update
    //Update Buttons
    Button &playerDeckButton = playingScene.playerDeckButton;
    UpdateButtonState(playerDeckButton,
                      mousePosition,
                      IsMouseButtonDown(MOUSE_BUTTON_LEFT),
                      IsMouseButtonReleased(MOUSE_BUTTON_LEFT));
    if (playerDeckButton.wasPressed)
    {
        PlaySound(GameSound::buttonPress01);
        DrawCardsFromDeckToHand(player1, 1);
    }
    //Update Music
    PlayMusic(playingScene.music);

    //Running the gameplay
    UpdateGameplayPhases(currentPhase, player1, player2, goingFirst);

    int hoveredCardIndex = -1;
    bool p1HoldingCard{player1.heldCardIndex > -1};
    static raylib::Vector2 heldCardOffset{};
    //Position player 1 hand cards in the hand zone
    for (size_t i = 0; i < player1.hand.size(); ++i)
    {
        if (CheckCollisionPointCard(mousePosition, player1.hand.at(i)))
        {
            hoveredCardIndex = static_cast<int>(i);
        }
        if (p1HoldingCard && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointCard(mousePosition, player1.hand.at(i)))
            {
                player1.heldCardIndex = static_cast<int>(i);
                heldCardOffset = mousePosition - player1.hand.at(i).pos;
            }
        }
        if (p1HoldingCard && player1.heldCardIndex == static_cast<int>(i))
        {
            player1.hand.at(i).pos = mousePosition - heldCardOffset;
            continue;
        }

        //Set the other cards in the hand slightly apart from each other.
        const int int_i = static_cast<int>(i);
        const raylib::Rectangle cardRect{
            static_cast<float>(constants::handZonePosX + 2 * int_i + 1 + constants::cardWidth * int_i),
            static_cast<float>(constants::handZonePosY + 4),
            constants::cardWidth,
            constants::cardHeight
        };

        player1.hand.at(i).pos = raylib::Vector2{cardRect.x, cardRect.y};
        player1.hand.at(i).faceUp = true;
    }
    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
    {
        // if (CanPlayHeldCard())
        // {
        //     m_gameManager.SetPlayerOneSelectedCardFromHand(m_heldCardIndex);
        // }

        //Reset Held-Card values
        player1.heldCardIndex = -1;
        heldCardOffset = Vector2Zero();
    }


    //Draw
    GetTexture(playingScene.background).Draw();
    DrawButton(playerDeckButton, GetTexture(playerDeckButton.background));

    //Draw Played Cards
    if (player1.cardInPlay.type != CardType::invalid)
    {
        Card cardInPlayCopy = player1.cardInPlay;
        cardInPlayCopy.pos = raylib::Vector2
        {
            constants::playerOnePlayfieldCardZoneRect.x,
            constants::playerOnePlayfieldCardZoneRect.y
        };
        DrawCard(cardInPlayCopy);
    }
    if (player2.cardInPlay.type != CardType::invalid)
    {
        Card cardInPlayCopy = player2.cardInPlay;
        cardInPlayCopy.pos = raylib::Vector2
        {
            constants::playerTwoCardPlayfieldPosX,
            constants::playerTwoCardPlayfieldPosY
        };
        DrawCard(cardInPlayCopy);
    }
    // Drawing Preview Zone Card
    if (p1HoldingCard)
    {
        DrawCardPreview(player1.hand.at(static_cast<std::size_t>(player1.heldCardIndex)));
    }
    else if (hoveredCardIndex != -1)
    {
        DrawCardPreview(player1.hand.at(static_cast<std::size_t>(hoveredCardIndex)));
    }
    // Draw Cards in Hand, with the one being held drawn last, on top.
    for (size_t i = 0; i < player1.hand.size(); ++i)
    {
        if (player1.heldCardIndex == static_cast<int>(i)) continue;
        DrawCard(player1.hand.at(i));
    }
    if (holdingCard)
    {
        DrawCard(player1.hand.at(static_cast<std::size_t>(player1.heldCardIndex)));
    }
}
