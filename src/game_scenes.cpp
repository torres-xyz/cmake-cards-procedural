#include "game_scenes.hpp"

#include "audio.hpp"
#include "game_play_phases.hpp"
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

void RunPlayingScene(PlayingScene &playingScene, const Vector2 &mousePosition,
                     GameplayPhase &currentPhase, Player &player1, Player &player2, int goingFirst)
{
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
    }
    //Update Music
    PlayMusic(playingScene.music);

    //Running the gameplay
    UpdateGameplayPhases(currentPhase, player1, player2, goingFirst);


    //Draw
    GetTexture(playingScene.background).Draw();
    DrawButton(playerDeckButton, GetTexture(playerDeckButton.background));
}
