#pragma once
#include <random>
#include "button.hpp"

struct Player;
enum class GameplayPhase;
enum class GameMusic;
enum class GameTexture;

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

void RunPlayingScene(PlayingScene &playingScene, GameplayPhase &currentPhase,
                     Player &player1, Player &player2, int goingFirst, std::random_device &rd);

void RunGameOverScene(GameOverScene &gameOverScene, GameScene &currentScene, GameplayPhase &gameplayPhase,
                      const Player &player1, const Player &player2);

void RunPrototypingScene(const PrototypingScene &scene);
