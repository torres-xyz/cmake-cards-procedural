#include <random> // for std::random_device
#include "constants.hpp"
#include "raylib-cpp.hpp"
#include "imgui_sidebar.hpp"
#include "rlImGui.h"
#include "button.hpp"
#include "audio.hpp"
#include "game_rules.hpp"
#include "textures.hpp"
#include "game_scenes.hpp"
#include "game_status.hpp"
#include "player.hpp"
#include "csv.h"
#include "game_turn.hpp"

int run()
{
    // Initialization ----------------------------------------------------------

    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTraceLogLevel(LOG_ERROR);
    const raylib::Window window(constants::windowScreenWidth, constants::windowScreenHeight, "Raylib-cpp Template");
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
    constexpr float nextRoundButtonWidth{200};
    constexpr float nextRoundButtonHeight{100};
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
            .state = ButtonState::disabled
        },
        .mulliganButton
        {
            .rectangle
            {
                constants::screenWidth - constants::cardWidth * 2 - 50,
                constants::screenHeight - constants::cardHeight - constants::handZoneBottomPadding,
                constants::cardWidth * 2,
                constants::cardHeight,
            },
            .text = "Mulligan",
            .fontSize = 20,
            .background = GameTexture::panel01,
            .state = ButtonState::disabled
        },
        .endTurnButton
        {
            .rectangle
            {
                constants::screenWidth - 150 - 50,
                380,
                180,
                60,
            },
            .text = "End Turn",
            .fontSize = 20,
            .background = GameTexture::panel05,
            .state = ButtonState::disabled
        },
        .nextRoundButton
        {
            .rectangle = raylib::Rectangle
            {
                constants::screenWidth * 0.5f - nextRoundButtonWidth * 0.5f,
                constants::screenHeight * 0.5f - nextRoundButtonHeight * 0.5f,
                nextRoundButtonWidth,
                nextRoundButtonHeight
            },
            .text = "Next Round",
            .fontSize = 20,
            .background = GameTexture::panel01,
            .state = ButtonState::enabled
        }
    };
    //GameScene::playing end ---------------------------------------------------
    //GameScene::gameOver start ------------------------------------------------
    constexpr float playAgainButtonWidth{200};
    constexpr float playAgainButtonHeight{100};
    GameOverScene gameOverScene
    {
        .gameScene = GameScene::gameOver,
        .background = GameTexture::wood11,
        .music = GameMusic::gameOver,
        .playAgainButton
        {
            .rectangle = raylib::Rectangle
            {
                constants::screenWidth * 0.5f - playAgainButtonWidth * 0.5f,
                constants::screenHeight * 0.5f - playAgainButtonHeight * 0.5f,
                playAgainButtonWidth,
                playAgainButtonHeight
            },
            .text = "Restart Game",
            .fontSize = 20,
            .background = GameTexture::panel01,
            .state = ButtonState::enabled
        }
    };
    //GameScene::gameOver end --------------------------------------------------

    //Prototyping Scene --------------------------------------------------------
    constexpr bool usePrototypingScene{false};
    if (usePrototypingScene)
    {
        currentScene = GameScene::prototyping;
    }
    PrototypingScene prototypingScene
    {
        .background = GameTexture::wood11
    };
    //GameScene::prototyping end -----------------------------------------------

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
