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

void RunPlayingScene(PlayingScene &playingScene, const raylib::Vector2 &mousePosition,
                     GameplayPhase &currentPhase, Player &player1, Player &player2, const int goingFirst)
{
    static bool player1HasDrawnThisTurn;

    auto UpdateDeckButton
    {
        [&player1, currentPhase, &playingScene, mousePosition]() {
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


    //Update Music
    PlayMusic(playingScene.music);

    //Running the gameplay
    [[maybe_unused]] int PlayerOneSelectedCardFromHand = -1;
    UpdateGameplayPhases(currentPhase, player1, player2, goingFirst, player1HasDrawnThisTurn);

    //Scene Buttons
    UpdateDeckButton();

    //Handle Card Positioning
    static raylib::Vector2 heldCardOffset{};
    int hoveredCardIndex{-1};

    //If we let go of the LMB we let go of the held card
    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
    {
        player1.heldCardIndex = -1;
    }
    //Place cards in the Hand Zone or Update Held Card
    for (size_t i = 0; i < player1.hand.size(); ++i)
    {
        //Move the card we are holding with the mouse
        if (i == static_cast<size_t>(player1.heldCardIndex))
        {
            player1.hand.at(i).pos = mousePosition - heldCardOffset;
            continue;
        }

        //If trying to click and hold a card
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
            CheckCollisionPointCard(mousePosition, player1.hand.at(i)) &&
            player1.heldCardIndex == -1)
        {
            player1.heldCardIndex = static_cast<int>(i);
            heldCardOffset = mousePosition - player1.hand.at(i).pos;

            //Don't position this card in the hand zone.
            continue;
        }

        //Set cards in the hand slightly apart from each other.
        const int int_i = static_cast<int>(i);
        const raylib::Rectangle cardRect
        {
            static_cast<float>(constants::handZonePosX + 2 * int_i + 1 + constants::cardWidth * int_i),
            static_cast<float>(constants::handZonePosY + 4),
            constants::cardWidth,
            constants::cardHeight
        };

        player1.hand.at(i).pos = raylib::Vector2{cardRect.x, cardRect.y};
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

    //Draw Hovered Card or Held Card in the Preview Zone
    if (hoveredCardIndex != -1 || player1.heldCardIndex != -1)
    {
        //Prefer showing our held card.
        int cardIndexToDisplay{};
        if (player1.heldCardIndex != -1) cardIndexToDisplay = player1.heldCardIndex;
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

    //Draw Cards in Hand, ...
    for (size_t i = 0; i < player1.hand.size(); ++i)
    {
        if (player1.heldCardIndex == static_cast<int>(i)) continue;
        DrawCard(player1.hand.at(i));
    }
    //... with the one being held drawn last, on top.
    if (player1.heldCardIndex != -1)
    {
        DrawCard(player1.hand.at(static_cast<std::size_t>(player1.heldCardIndex)));
    }
}
