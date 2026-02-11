#pragma once
#include <random>

#include "audio.hpp"
#include "button.hpp"
#include "game_status.hpp"
#include "textures.hpp"

struct GameStatus;
struct GameRules;
struct Player;
enum class GameplayPhase;
enum class GameMusic;
enum class GameTexture;
enum class TurnPhase;

enum class GameScene
{
    invalid,
    starting,
    playing,
    gameOver,
    prototyping
};

struct StartingScene
{
    GameScene gameScene{GameScene::starting};
    GameTexture background{GameTexture::metal35};
    GameMusic music{GameMusic::start};
    Button startButton
    {
        .rectangle = raylib::Rectangle
        {
            constants::screenWidth * 0.5f - 200 * 0.5f,
            constants::screenHeight * 0.5f - 100 * 0.5f,
            200,
            100
        },
        .text = "Start Game",
        .fontSize = 20,
        .background = GameTexture::panel01,
        .state = ButtonState::enabled
    };
};

struct PlayingScene
{
    GameScene gameScene{GameScene::playing};
    GameTexture background{GameTexture::wall13};
    GameTexture cardPreviewZoneTex{GameTexture::metal08};
    GameTexture playfield{GameTexture::wood11};
    GameMusic music{GameMusic::playing};
    Button playerDeckButton
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
    };
    Button mulliganButton
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
    };
    Button endTurnButton
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
    };
    Button nextRoundButton
    {
        .rectangle = raylib::Rectangle
        {
            constants::screenWidth * 0.5f - 200 * 0.5f,
            constants::screenHeight * 0.5f - 100 * 0.5f,
            200,
            100
        },
        .text = "Next Round",
        .fontSize = 20,
        .background = GameTexture::panel01,
        .state = ButtonState::disabled
    };
    Button undoPlayerActionButton{

        .rectangle = raylib::Rectangle
        {
            constants::screenWidth - 150 - 50,
            480,
            180,
            60,
        },
        .text = "Undo",
        .fontSize = 20,
        .background = GameTexture::panel01,
        .state = ButtonState::enabled
    };
};

struct GameOverScene
{
    GameScene gameScene{GameScene::gameOver};
    GameTexture background{GameTexture::wood11};
    GameMusic music{GameMusic::gameOver};
    Button playAgainButton
    {
        .rectangle = raylib::Rectangle
        {
            constants::screenWidth * 0.5f - 200 * 0.5f,
            constants::screenHeight * 0.5f - 100 * 0.5f,
            200,
            100
        },
        .text = "Restart Game",
        .fontSize = 20,
        .background = GameTexture::panel01,
        .state = ButtonState::enabled
    };
};

struct PrototypingScene
{
    GameTexture background{GameTexture::wood11};
};

void RunStartingScene(StartingScene &startingScene, GameScene &currentScene, Player &player1, Player &player2, GameStatus &gameStatus);

void RunPlayingScene(PlayingScene &playingScene, TurnPhase &currentTurnPhase, GameStatus &gameStatus,
                     Player &player1, Player &player2, const GameRules &gameRules);

void RunGameOverScene(GameOverScene &gameOverScene, GameScene &currentScene, TurnPhase &turnPhase, GameStatus &gameStatus,
                      Player &player1, Player &player2);

void RunPrototypingScene(const PrototypingScene &scene);

std::string GameSceneToString(const GameScene &gameScene);
