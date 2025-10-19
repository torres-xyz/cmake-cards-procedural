#include <random> // for std::random_device
#include "constants.hpp"
#include "raylib-cpp.hpp"
#include "imgui_sidebar.hpp"
#include "rlImGui.h"
#include "button.hpp"
#include "audio.hpp"
#include "game_play_phases.hpp"
#include "textures.hpp"
#include "game_scenes.hpp"
#include "player.hpp"

int run()
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

    //Player 1
    Player player1{.id = 1};
    Player player2{.id = 2};

    InitializePlayer(player1, rd);
    InitializePlayer(player2, rd);
    constexpr int playerGoingFirst{2};
    GameScene currentScene{GameScene::starting};
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
        .playfield = GameTexture::wood11,
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
    //GameScene::playing end ---------------------------------------------------
    //GameScene::gameOver start ------------------------------------------------
    constexpr int playAgainButtonWidth{200};
    constexpr int playAgainButtonHeight{100};
    GameOverScene gameOverScene
    {
        .gameScene = GameScene::gameOver,
        .background = GameTexture::metal22,
        .music = GameMusic::gameOver,
        .playAgainButton
        {
            .rectangle = raylib::Rectangle
            {
                constants::screenWidth * 0.5f - static_cast<float>(playAgainButtonWidth) * 0.5f,
                constants::screenHeight * 0.5f - static_cast<float>(playAgainButtonHeight) * 0.5f,
                static_cast<float>(playAgainButtonWidth),
                static_cast<float>(playAgainButtonHeight)
            },
            .text = "Restart Game",
            .fontSize = 20,
            .background = GameTexture::panel01,
            .state = ButtonState::enabled
        }
    };
    //GameScene::gameOver end --------------------------------------------------

    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        [[maybe_unused]] Vector2 mousePos = GetMousePosition();
        SetMasterVolume(!muteGame);

        switch (currentScene)
        {
            case GameScene::invalid:
                break;
            case GameScene::starting:
            {
                RunStartingScene(startingScene, currentScene);
                break;
            }
            case GameScene::playing:
            {
                RunPlayingScene(playingScene, currentPhase,
                                player1, player2, playerGoingFirst, rd);

                if (currentPhase == GameplayPhase::gameOver)
                {
                    currentScene = GameScene::gameOver;
                }
                break;
            }
            case GameScene::gameOver:
            {
                RunGameOverScene(gameOverScene, currentScene, currentPhase);
                break;
            }
        }


#if (DEBUG)
        ImGuiSideBar::DrawSideBar(muteGame, currentPhase, player1, player2);
#endif
        EndDrawing();

        // De-Initialization -------------------------------------------------------
        // UnloadTexture() and CloseWindow() are called automatically.
    };


    return 0;
}
