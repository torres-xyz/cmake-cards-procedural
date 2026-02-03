#include "cpu_brain.hpp"

#include "player.hpp"

void RunCpuBrain(Player &player, const CpuPlayerOptions &options)
{
    static float timeSinceLastAction{};

    timeSinceLastAction += GetFrameTime();
    if (timeSinceLastAction < options.actionDelay) return;

    // Look at the available actions and pick one.
    if (player.availableActions.empty()) return;

    for (const PlayerActionAndHandCardPair &actionCardPair: player.availableActions)
    {
        if (actionCardPair.action == PlayerAction::null) continue; // Nothing to do.
        if (actionCardPair.action == PlayerAction::forfeit) continue; // Never forfeit.

        //Always draw a card first
        if (actionCardPair.action == PlayerAction::drawCard)
        {
            player.chosenAction = actionCardPair;
            break;
        }
        //Mulligan when available
        if (actionCardPair.action == PlayerAction::mulligan)
        {
            player.chosenAction = actionCardPair;
            break;
        }

        // Play the first Unit that shows up in the available actions
        if (actionCardPair.action == PlayerAction::playCard)
        {
            player.chosenAction = actionCardPair;
            break;
        }

        //If there is nothing else to do, pass the turn
        if (actionCardPair.action == PlayerAction::passTheTurn)
        {
            player.chosenAction = actionCardPair;
            break;
        }

        //Start a new Round
        if (options.autoStartNewRound)
        {
            if (actionCardPair.action == PlayerAction::finishRound)
            {
                player.chosenAction = actionCardPair;
                break;
            }
        }
    }

    timeSinceLastAction = 0;
}
