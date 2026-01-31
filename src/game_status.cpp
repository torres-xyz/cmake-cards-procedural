#include "game_status.hpp"

#include <format>
#include <string>

#include "game_turn.hpp"


std::string ActionLogEntryToString(const ActionLog &log)
{
    std::string printedLog
    {
        std::format("R-{0}, T-{1}, P-{2}, Ph-'{3}' did '{4}'",
                    std::to_string(log.roundNumber),
                    std::to_string(log.turnNumber),
                    std::to_string(log.playerID),
                    TurnPhaseToString(log.turnPhase),
                    PlayerActionToString(log.actionCardPairTaken.action))
    };
    if (log.actionCardPairTaken.action == PlayerAction::playCard)
    {
        printedLog.append(" - " + log.actionCardPairTaken.card.name);
    }

    return printedLog;
}
