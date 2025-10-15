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

void RunPlayingScene(PlayingScene &playingScene, const Vector2 &mousePosition,
                     GameplayPhase &currentPhase, Player &player1, Player &player2, int goingFirst)
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

    //Place player 1 hand cards on the screen

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
