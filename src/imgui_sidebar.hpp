#pragma once

enum class GameScene;
enum class TurnPhase;
struct GameStatus;
struct Player;

namespace ImGuiSideBar
{
    void DrawSideBar(bool &muteGame, const GameScene &currentGameScene, const TurnPhase &currentTurnPhase, const GameStatus &gameStatus, Player &player1, Player &player2);
}
