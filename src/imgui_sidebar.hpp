#pragma once

enum class GameScene;
enum class TurnPhase;
struct GameStatus;
struct Player;

namespace ImGuiSideBar
{
    void DrawSideBar(bool &muteGame, const GameScene &currentGameScene, const TurnPhase &currentTurnPhase, GameStatus &gameStatus, Player &player1, const Player &player2);
}
