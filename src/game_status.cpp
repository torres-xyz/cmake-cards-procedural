#include "game_status.hpp"

#include <format>
#include <string>

#include "game_turn.hpp"


std::string ActionLogEntryToString(ActionLog log)
{
    std::string printedLog
    {
        std::format("[R: {0}, T: {1}] During {2} Player {3} did {4} - {5}",
                    std::to_string(log.roundNumber),
                    std::to_string(log.turnNumber),
                    TurnPhaseToString(log.turnPhase),
                    std::to_string(log.playerID),
                    PlayerActionToString(log.actionCardPairTaken.action),
                    log.actionCardPairTaken.card.name)
    };

    return printedLog;
}
