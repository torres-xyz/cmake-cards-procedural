#include "game_turn.hpp"

#include <cassert>
#include <format>

#include "audio.hpp"
#include "game_rules.hpp"
#include "game_status.hpp"
#include "player.hpp"

std::vector<PlayerActionAndHandCardPair> CalculateAvailableActions(const Player &player, const TurnPhase &turnPhase, const GameRules &gameRules, const GameStatus &gameStatus)
{
    std::vector<PlayerActionAndHandCardPair> possibleActions{};

    possibleActions.push_back({PlayerAction::forfeit});

    switch (turnPhase)
    {
        case TurnPhase::initialSetup:
        {
            if (static_cast<int>(player.hand.size()) < gameRules.initialHandSize)
            {
                possibleActions.push_back({PlayerAction::drawCard});
            }
            if (static_cast<int>(player.hand.size()) == gameRules.initialHandSize)
            {
                if (IsInitialHandValid(player.hand, gameRules))
                {
                    //pass the turn?
                }
                else
                {
                    possibleActions.push_back({PlayerAction::mulligan});
                }
            }
            break;
        }
        case TurnPhase::drawStep:
        {
            //If player can draw a card, do it
            if (!player.deck.empty())
            {
                possibleActions.push_back({PlayerAction::drawCard});
            }
            break;
        }
        case TurnPhase::mainPhase:
        {
            auto HasPlayedACardThisTurn = [](const GameStatus &gS) -> bool {
                const int currentTurn = gS.turnsPlayed;
                for (int i = static_cast<int>(gS.actionLogs.size()) - 1; i >= 0; --i)
                {
                    if (gS.actionLogs.at(static_cast<size_t>(i)).turnNumber == currentTurn &&
                        gS.actionLogs.at(static_cast<size_t>(i)).actionCardPairTaken.action == PlayerAction::playCard)
                    {
                        return true;
                    }
                }
                return false;
            };


            //If the player has NO cards in play, they MUST play a Unit
            if (player.cardsInPlayStack.empty())
            {
                //Look for Units in the player's hand.
                bool foundAUnitToPlay{false};
                for (const Card &card: player.hand)
                {
                    if (card.type == CardType::unit)
                    {
                        possibleActions.push_back({PlayerAction::playCard, card});
                        foundAUnitToPlay = true;
                    }
                }
                if (foundAUnitToPlay == false)
                {
                    //If the player can't play another Unit, they lose the game.
                }
            }

            // You can play Action Cards on your Unit card in play.
            if (!player.cardsInPlayStack.empty() &&
                !HasPlayedACardThisTurn(gameStatus) &&
                player.cardsInPlayStack.at(0).type == CardType::unit)
            {
                for (const Card &card: player.hand)
                {
                    if (card.type == CardType::action)
                    {
                        possibleActions.push_back({PlayerAction::playCard, card});
                    }
                }
            }

            //Can only pass the turn if player has a Unit in play
            if (!player.cardsInPlayStack.empty() && player.cardsInPlayStack.at(0).type == CardType::unit)
            {
                possibleActions.push_back({PlayerAction::passTheTurn});
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
    auto HasPlayerDrawnForTheTurnThisTurn = [](const GameStatus &gS, const Player &p)-> bool {
        //Needs to take into account previous actions
        if (const ActionLog lastLog = gS.actionLogs.back();
            lastLog.turnPhase == TurnPhase::drawStep &&
            lastLog.playerID == p.id &&
            lastLog.turnNumber == gS.turnsPlayed)
        {
            return true;
        }

        return false;
    };


    switch (currentTurnPhase)
    {
        case TurnPhase::initialSetup: //only executed once per round.
        {
            ExecuteChosenPlayerAction(player, currentTurnPhase, gameStatus);

            // Auto shuffle deck ?

            // Wait for initial hand draw
            if (IsInitialHandValid(player.hand, gameRules))
            {
                currentTurnPhase = TurnPhase::drawStep;
            }

            break;
        }
        case TurnPhase::drawStep:
        {
            ExecuteChosenPlayerAction(player, currentTurnPhase, gameStatus);

            // Wait for draw.
            if (HasPlayerDrawnForTheTurnThisTurn(gameStatus, player))
            {
                currentTurnPhase = TurnPhase::mainPhase;
            }

            break;
        }
        case TurnPhase::mainPhase:
        {
            ExecuteChosenPlayerAction(player, currentTurnPhase, gameStatus);

            break;
        }
        case TurnPhase::endPhase:
        {
            //If both players passed, Battle

            break;
        }
    }
}


void ExecuteChosenPlayerAction(Player &player, const TurnPhase &turnPhase, GameStatus &gameStatus)
{
    auto LogAction = [](const Player &p, const TurnPhase &tP, GameStatus &gS) {
        gS.actionLogs.push_back(
            {
                p.chosenAction,
                tP,
                gS.turnsPlayed,
                gS.roundsPlayed,
                p.id
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
            return;
        }
        case PlayerAction::drawCard:
        {
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
        {
            //Copy the card from the hand to the Play field.
            for (size_t i = 0; i < player.hand.size(); ++i)
            {
                if (player.hand.at(i).uid != player.chosenAction.card.uid)
                    continue;

                player.cardsInPlayStack.emplace_back(player.hand.at(i));
                //Then remove that card from the hand.
                player.hand.erase(player.hand.begin() + static_cast<long int>(i));
                break;
            }

            PlaySound(GameSound::cardPlace01);

            break;
        }
        case PlayerAction::passTheTurn:
        {
            gameStatus.currentTurnOwner = player.id == 1 ? 2 : 1;
            gameStatus.turnsPlayed++;

            break;
        }
        case PlayerAction::forfeit:
            break;
    }

    LogAction(player, turnPhase, gameStatus);
    player.chosenAction = {PlayerAction::null};
}

std::string PlayerActionToString(const PlayerAction playerAction)
{
    switch (playerAction)
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
            return "Play Card";
        case PlayerAction::passTheTurn:
            return "Pass The Turn";
        case PlayerAction::forfeit:
            return "Forfeit";
    }
    return "ERROR";
}

std::string TurnPhaseToString(const TurnPhase phase)
{
    switch (phase)
    {
        case TurnPhase::initialSetup:
            return "Initial Setup";
        case TurnPhase::drawStep:
            return "Draw Step";
        case TurnPhase::mainPhase:
            return "Main Phase";
        case TurnPhase::endPhase:
            return "End Phase";
    }
    return "null";
}
