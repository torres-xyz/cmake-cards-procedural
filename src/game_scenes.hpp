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

void RunStartingScene(StartingScene &startingScene, GameScene &currentScene);

void RunPlayingScene(PlayingScene &playingScene, TurnPhase &currentTurnPhase, const GameplayPhase &currentPhase, GameStatus &gameStatus,
                     Player &player1, Player &player2, const GameRules &gameRules, std::random_device &rd);

void RunGameOverScene(GameOverScene &gameOverScene, GameScene &currentScene, GameplayPhase &gameplayPhase,
                      const Player &player1, const Player &player2);

void RunPrototypingScene(const PrototypingScene &scene);
