#include <iostream>
#include <random> // for std::random_device
#include "constants.hpp"
// #include "raylib.h"
#include <map>
#include "raylib-cpp.hpp"
#include "imgui.h"
#include "imgui_sidebar.hpp"
#include "rlImGui.h"
#include "button.hpp"
#include "audio.hpp"
#include "game_play_phases.hpp"
#include "textures.hpp"
#include "game_scenes.hpp"
#include "player.hpp"


int main()
{
    // Initialization ----------------------------------------------------------
    std::random_device rd{};
    SetRandomSeed(rd());

    SetConfigFlags(FLAG_VSYNC_HINT);
    const raylib::Window window(constants::windowScreenWidth, constants::windowScreenHeight, "Raylib-cpp Template");
    SetTargetFPS(GetMonitorRefreshRate(0));
#if (DEBUG)
    rlImGuiSetup(true);
#endif
    InitAudioDevice();
    bool muteGame{true};

    GameScene currentScene{GameScene::starting};

    //Player 1
    Player player1
    {
        .id = 1,
        .score = 0
    };
    Player player2
    {
        .id = 2,
        .score = 0
    };

    InitializePlayer(player1, rd);
    InitializePlayer(player2, rd);
    int playerGoingFirst{2};
    GameplayPhase currentPhase{GameplayPhase::initialHandDraw};


    //GameScene::start ---------------------------------------------------------
    constexpr int startGameButtonWidth{200};
    constexpr int startGameButtonHeight{100};
    StartingScene startingScene
    {
        .gameScene = GameScene::starting,
        .background = GameTexture::metal35,
        .music = GameMusic::start,
        .startButton
        {
            .rectangle = raylib::Rectangle
            {
                constants::screenWidth * 0.5f - static_cast<float>(startGameButtonWidth) * 0.5f,
                constants::screenHeight * 0.5f - static_cast<float>(startGameButtonHeight) * 0.5f,
                static_cast<float>(startGameButtonWidth),
                static_cast<float>(startGameButtonHeight)
            },
            .text = "Star Game",
            .fontSize = 20,
            .background = GameTexture::panel01,
            .state = ButtonState::enabled
        }
    };

    //GameScene::start end -----------------------------------------------------
    //GameScene::playing -------------------------------------------------------
    PlayingScene playingScene
    {
        .gameScene = GameScene::playing,
        .background = GameTexture::wall13,
        .cardPreviewZoneTex = GameTexture::metal08,
        .music = GameMusic::playing,
        .playerDeckButton
        {
            .rectangle
            {
                constants::screenWidth - constants::cardWidth - 50,
                constants::screenHeight - constants::cardHeight - constants::handZoneBottomPadding,
                constants::cardWidth,
                constants::cardHeight,
            },
            .text = "Deck",
            .fontSize = 20,
            .background = GameTexture::panel01,
            .state = ButtonState::enabled
        },
    };

    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        Vector2 mousePos = GetMousePosition();
        SetMasterVolume(!muteGame);

        switch (currentScene)
        {
            case GameScene::invalid:
                break;
            case GameScene::starting:
            {
                RunStartingScene(startingScene, currentScene, mousePos);
                break;
            }
            case GameScene::playing:
            {
                RunPlayingScene(playingScene, mousePos, currentPhase,
                                player1, player2, playerGoingFirst);
                break;
            }
            case GameScene::gameOver:
                break;
        }


#if (DEBUG)
        ImGuiSideBar::DrawSideBar(muteGame);
#endif
        EndDrawing();

        // De-Initialization -------------------------------------------------------
        // UnloadTexture() and CloseWindow() are called automatically.
    };


    return 0;
}
