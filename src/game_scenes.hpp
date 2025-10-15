#pragma once
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
    gameOver
};

struct Scene //base for reference?
{
    GameScene gameScene{};
    GameTexture background{};
    GameMusic music{};
};

struct StartingScene
{
    GameScene gameScene{};
    GameTexture background{};
    GameMusic music{};
    Button startButton{};
};

struct PlayingScene
{
    GameScene gameScene{};
    GameTexture background{};
    GameTexture cardPreviewZoneTex{};
    GameMusic music{};
    Button playerDeckButton{};
};

void RunStartingScene(StartingScene &startingScene,
                      GameScene &currentScene,
                      const Vector2 &mousePosition);

void RunPlayingScene(PlayingScene &playingScene, const raylib::Vector2 &mousePosition,
                     GameplayPhase &currentPhase, Player &player1, Player &player2,
                     int goingFirst);


