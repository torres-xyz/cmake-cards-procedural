#include "imgui_sidebar.hpp"
#include <iostream>
#include "imgui.h"
#include "rlImGui.h"
#include "raylib-cpp.hpp"
#include "constants.hpp"
#include "game_scenes.hpp"
#include "game_status.hpp"
#include "helper_functions.hpp"
#include "player.hpp"

namespace ImGuiSideBar
{
    void DrawSideBar(bool &muteGame, const GameScene &currentGameScene, const TurnPhase &currentTurnPhase, GameStatus &gameStatus,
                     Player &player1, const Player &player2)
    {
        // start ImGui Content
        rlImGuiBegin();

        // show ImGui Content
        bool open = false;
        ImGui::SetNextWindowPos(ImVec2(constants::windowScreenWidth - constants::imguiSideBarWidth,
                                       0));
        ImGui::SetNextWindowSize(
            ImVec2(constants::imguiSideBarWidth, constants::windowScreenHeight));
        if (ImGui::Begin("Test Window", &open,
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoTitleBar))
        {
            ImGui::TextUnformatted(("FPS: " + std::to_string(GetFrameTime())).c_str());
            ImGui::TextUnformatted(("Mouse X: " + std::to_string(GetMouseX())
                                    + " / Mouse Y: " + std::to_string(GetMouseY())).c_str());
            ImGui::SeparatorText("Game State");
            ImGui::TextUnformatted(("Game Scene = " + GameSceneToString(currentGameScene)).c_str());
            ImGui::TextUnformatted(("Player 1 points = " + std::to_string(gameStatus.pointsPlayer1)).c_str());
            ImGui::TextUnformatted(("Player 1 hand = " + std::to_string(player1.hand.size())).c_str());
            ImGui::TextUnformatted(("Player 1 deck = " + std::to_string(player1.deck.size())).c_str());
            ImGui::TextUnformatted(("Player 2 points = " + std::to_string(gameStatus.pointsPlayer2)).c_str());
            ImGui::TextUnformatted(("Player 2 hand = " + std::to_string(player2.hand.size())).c_str());
            ImGui::TextUnformatted(("Player 2 deck = " + std::to_string(player2.deck.size())).c_str());
            ImGui::SeparatorText("Turn Phase");
            ImGui::TextUnformatted(("Turn Owner = " + std::to_string(gameStatus.currentTurnOwner)).c_str());
            ImGui::TextUnformatted(("Turn Phase = " + TurnPhaseToString(currentTurnPhase)).c_str());
            ImGui::SeparatorText("Player 1 available actions");
            ImGui::TextUnformatted(HelperFunctions::PlayerAvailableActionsToString(player1).c_str());
            ImGui::SeparatorText("Options");
            ImGui::Checkbox("Mute", &muteGame);
            ImGui::SeparatorText("Debug Actions");
            if (ImGui::Button("Inject Deck in Player 1"))
            {
                DEBUG_InjectDeckInPlayer(player1, templeShieldDeck);
            }
            ImGui::SeparatorText("Actions Log");
            ImGui::TextWrapped("%s", HelperFunctions::ActionLogsToString(gameStatus).c_str());

            ImGui::TextUnformatted("My Text inside the window");
        }
        ImGui::End();

        // ImGui::ShowDemoWindow();
        // end ImGui Content
        rlImGuiEnd();
    }
}
