#include "game_status.hpp"

#include <format>
#include <string>

#include "game_turn.hpp"


std::string ActionLogEntryToString(ActionLog log)
{
    std::string printedLog
    {
        std::format("[R: {0}, T: {1}] Player {2} did {3} - {4}",
                    std::to_string(log.roundNumber),
                    std::to_string(log.turnNumber),
                    std::to_string(log.playerID),
                    PlayerActionToString(log.actionCardPairTaken),
                    log.details)
    };

    return printedLog;
}
