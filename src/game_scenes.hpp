#pragma once
#include <random>
#include "button.hpp"
#include "game_status.hpp"

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
    GameTexture background{};
    GameMusic music{};
    Button startButton{};
};

struct PlayingScene
{
    GameScene gameScene{GameScene::playing};
    GameTexture background{};
    GameTexture cardPreviewZoneTex{};
    GameTexture playfield{};
    GameMusic music{};
    Button playerDeckButton{};
    Button mulliganButton{};
    Button endTurnButton{};
    Button nextRoundButton{};
};

struct GameOverScene
{
    GameScene gameScene{GameScene::gameOver};
    GameTexture background{};
    GameMusic music{};
    Button playAgainButton{};
};

struct PrototypingScene
{
    GameTexture background{};
};

void RunStartingScene(StartingScene &startingScene, GameScene &currentScene, Player &player1, Player &player2, GameStatus &gameStatus);

void RunPlayingScene(PlayingScene &playingScene, TurnPhase &currentTurnPhase, GameStatus &gameStatus,
                     Player &player1, Player &player2, const GameRules &gameRules);

void RunGameOverScene(GameOverScene &gameOverScene, GameScene &currentScene, TurnPhase &turnPhase, GameStatus &gameStatus,
                      Player &player1, Player &player2);

void RunPrototypingScene(const PrototypingScene &scene);
