#include "game_turn.hpp"

#include <cassert>

#include "game_rules.hpp"
#include "game_status.hpp"
#include "player.hpp"

std::vector<PlayerActionAndHandCardPair> CalculateAvailableActions(const Player &player, const TurnPhase &turnPhase, const GameRules &gameRules)
{
    std::vector<PlayerActionAndHandCardPair> possibleActions{};

    possibleActions.push_back({PlayerAction::forfeit, 0});

    switch (turnPhase)
    {
        case TurnPhase::initialSetup:
        {
            if (static_cast<int>(player.hand.size()) < gameRules.initialHandSize)
            {
                possibleActions.push_back({PlayerAction::drawCard, 0});
            }
            if (static_cast<int>(player.hand.size()) == gameRules.initialHandSize)
            {
                if (IsInitialHandValid(player.hand, gameRules))
                {
                    //pass the turn? doesn't make sense since this is the initial setup, both players are playing at once.
                }
                else
                {
                    possibleActions.push_back({PlayerAction::mulligan, 0});
                }
            }
            break;
        }
        case TurnPhase::startOfTheTurnDraw:
        {
            //If player can draw, draw
            if (!player.deck.empty())
            {
                possibleActions.push_back({PlayerAction::drawCard, 0});
            }
            break;
        }
        case TurnPhase::mainPhase:
        {
            //If the player has NO cards in play, they MUST play a Unit
            if (player.cardsInPlayStack.empty())
            {
                possibleActions.push_back({PlayerAction::playCard, 0}); //TODO: Change to a playable card
            }

            //Can only pass the turn if player has a Unit in play
            if (!player.cardsInPlayStack.empty() && player.cardsInPlayStack.at(0).type == CardType::unit)
            {
                possibleActions.push_back({PlayerAction::passTheTurn, 0});
            }

            break;
        }
        case TurnPhase::endPhase:
        {
            break;
        }
    }

    return possibleActions;
}

// Make use of the command pattern, so that it is easy to list actions, undo them, and redo them.
void ExecuteTurn(Player &player, TurnPhase &currentTurnPhase, const GameRules &gameRules, GameStatus &gameStatus)
{
    if (gameStatus.roundsPlayed == 0)
    {
        currentTurnPhase = TurnPhase::initialSetup;
    }
    switch (currentTurnPhase)
    {
        case TurnPhase::initialSetup: //only executed once per round.
        {
            ExecutePlayerAction(player, gameStatus);

            // Auto shuffle deck ?

            // Wait for initial hand draw
            if (IsInitialHandValid(player.hand, gameRules))
            {
                currentTurnPhase = TurnPhase::mainPhase;
            }

            break;
        }
        case TurnPhase::startOfTheTurnDraw:
        {
            // Wait for draw.
            // Check if a player has already drawn this turn by
            // looking at the actions log and looking for a drawCard action in their current turn.

            break;
        }
        case TurnPhase::mainPhase:
        {
            break;
        }
        case TurnPhase::endPhase:
        {
            break;
        }
    }
}


void ExecutePlayerAction(Player &player, GameStatus &gameStatus)
{
    auto LogAction = [](GameStatus &gS, const Player &p, const std::string &d) {
        gS.actionLogs.push_back(
            {
                p.chosenAction,
                gS.turnsPlayed,
                gS.roundsPlayed,
                p.id,
                d
            });
    };

    auto DrawCardsFromDeckToHand = [](Player &p, const int amount) {
        assert(amount <= static_cast<int>(p.deck.size())
            && "Trying to draw more cards than player has in its deck.\n");

        for (int i = 0; i < amount; ++i)
        {
            Card drawnCard = p.deck.at(0);
            drawnCard.rect.SetPosition(raylib::Vector2{-1000, -1000});
            drawnCard.faceUp = false;
            p.hand.push_back(drawnCard);
            p.deck.erase(p.deck.begin());
        }
    };

    switch (player.chosenAction.action)
    {
        case PlayerAction::null:
        {
            break;
        }
        case PlayerAction::drawCard:
        {
            LogAction(gameStatus, player, "");
            DrawCardsFromDeckToHand(player, 1);
            break;
        }
        case PlayerAction::mulligan:
            break;
        case PlayerAction::shuffleHandIntoDeck:
            break;
        case PlayerAction::shuffleDeck:
            break;
        case PlayerAction::playCard:
            break;
        case PlayerAction::passTheTurn:
            break;
        case PlayerAction::forfeit:
            break;
    }

    player.chosenAction = {PlayerAction::null, 0};
}

std::string PlayerActionToString(const PlayerActionAndHandCardPair actionCardPair)
{
    switch (actionCardPair.action)
    {
        case PlayerAction::null:
            return "Null";
        case PlayerAction::drawCard:
            return "Draw Card";
        case PlayerAction::mulligan:
            return "Mulligan";
        case PlayerAction::shuffleHandIntoDeck:
            return "ShuffleHandIntoDeck";
        case PlayerAction::shuffleDeck:
            return "Shuffle Deck";
        case PlayerAction::playCard:
            return "Play Card ";
        case PlayerAction::passTheTurn:
            return "Pass The Turn";
        case PlayerAction::forfeit:
            return "Forfeit";
    }
    return "ERROR";
}
