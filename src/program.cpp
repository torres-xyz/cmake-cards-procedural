#include "constants.hpp"
#include "raylib-cpp.hpp"
#include "imgui_sidebar.hpp"
#include "rlImGui.h"
#include "game_rules.hpp"
#include "game_scenes.hpp"
#include "game_status.hpp"
#include "player.hpp"
#include "csv.h"
#include "game_turn.hpp"

int run()
{
    // Initialization ----------------------------------------------------------
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTraceLogLevel(LOG_WARNING);

    const raylib::Window window(constants::windowScreenWidth, constants::windowScreenHeight, "C++ TCG");
    SetTargetFPS(GetMonitorRefreshRate(0));
#if (DEBUG)
    rlImGuiSetup(true);
#endif
    InitAudioDevice();
    bool muteGame{true};

    GetCardDB();

    //Player 1
    Player player1{.id = 1};
    Player player2{.id = 2};

    constexpr GameRules gameRules
    {
        .playerGoingFirst = 2,
        .pointsNeededToWin = 2 //Best of 3 game
    };

    GameScene currentScene{GameScene::starting};

    TurnPhase currentTurnPhase{TurnPhase::initialSetup};

    GameStatus gameStatus{};

    //Scenes
    StartingScene startingScene{};
    PlayingScene playingScene{};
    GameOverScene gameOverScene{};
    PrototypingScene prototypingScene{};

    if (constexpr bool usePrototypingScene{false}; usePrototypingScene)
    {
        currentScene = GameScene::prototyping;
    }

    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        SetMasterVolume(!muteGame);

        switch (currentScene)
        {
            case GameScene::invalid:
                break;
            case GameScene::starting:
            {
                RunStartingScene(startingScene, currentScene, player1, player2, gameStatus);
                break;
            }
            case GameScene::playing:
            {
                RunPlayingScene(playingScene, currentTurnPhase, gameStatus, player1, player2, gameRules);

                if (gameStatus.gameIsOver)
                {
                    currentScene = GameScene::gameOver;
                }
                break;
            }
            case GameScene::gameOver:
            {
                RunGameOverScene(gameOverScene, currentScene, currentTurnPhase, gameStatus, player1, player2);
                break;
            }
            case GameScene::prototyping:
            {
                RunPrototypingScene(prototypingScene);
                break;
            }
        }


#if (DEBUG)
        ImGuiSideBar::DrawSideBar(muteGame, currentScene, currentTurnPhase, gameStatus, player1, player2);
#endif
        EndDrawing();

        // De-Initialization -------------------------------------------------------
        // UnloadTexture() and CloseWindow() are called automatically.
    };


    return 0;
}
