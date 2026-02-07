#include "game_turn.hpp"

#include <algorithm>
#include <cassert>
#include <format>

#include "audio.hpp"
#include "game_rules.hpp"
#include "game_status.hpp"
#include "helper_functions.hpp"
#include "player.hpp"

// Forward declaration of internal functions.
void DrawCardsFromDeckToHand(Player &player, int amount);

bool HasACardBeenPlayedThisTurn(const GameStatus &gS)
{
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
}

bool HasACardBeenPlayedLastTurn(const GameStatus &gS)
{
    const int currentTurn = gS.turnsPlayed;
    for (int i = static_cast<int>(gS.actionLogs.size()) - 1; i >= 0; --i)
    {
        if (gS.actionLogs.at(static_cast<size_t>(i)).turnNumber == currentTurn - 1 &&
            gS.actionLogs.at(static_cast<size_t>(i)).actionCardPairTaken.action == PlayerAction::playCard)
        {
            return true;
        }
    }
    return false;
}


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
                if (!IsInitialHandValid(player.hand, gameRules))
                {
                    possibleActions.push_back({PlayerAction::mulligan});
                }
            }
            break;
        }
        case TurnPhase::mainPhase:
        {
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
                !HasACardBeenPlayedThisTurn(gameStatus) &&
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
        case TurnPhase::endTurnPhase:
        {
            break;
        }
        case TurnPhase::battlePhase:
        {
            // No actions will ever be available during the Battle Phase.
            break;
        }
        case TurnPhase::endRoundPhase:
        {
            //Start new Round
            possibleActions.push_back({PlayerAction::finishRound});

            //Start new game
            break;
        }
    }

    return possibleActions;
}

// Make use of the command pattern, so that it is easy to list actions, undo them, and redo them.
void ExecuteTurn(Player &player, Player &opponentPlayer, TurnPhase &currentTurnPhase, const GameRules &gameRules, GameStatus &gameStatus)
{
    assert(player.id != opponentPlayer.id && "Passing the same player as turn owner and opponent");

    //If there is a new turn owner
    if (!gameStatus.actionLogs.empty() && gameStatus.actionLogs.back().playerID != player.id)
    {
        //During the current round, have they had an Initial Setup Phase?
        bool hasHadInitialSetupPhaseThisRound{false};
        for (const ActionLog &actionLog: gameStatus.actionLogs)
        {
            if (actionLog.roundNumber == gameStatus.roundsPlayed &&
                actionLog.playerID == player.id &&
                actionLog.turnPhase == TurnPhase::initialSetup)
            {
                hasHadInitialSetupPhaseThisRound = true;
            }
        }

        if (hasHadInitialSetupPhaseThisRound)
        {
            // currentTurnPhase = TurnPhase::drawStep;
            currentTurnPhase = TurnPhase::mainPhase;
        }
        else
        {
            currentTurnPhase = TurnPhase::initialSetup;
        }
    }


    switch (currentTurnPhase)
    {
        case TurnPhase::initialSetup: //only executed once per round. but different
        {
            ExecuteChosenPlayerAction(player, currentTurnPhase, gameRules, gameStatus);

            // Auto shuffle deck ?

            // Wait for initial hand draw
            if (IsInitialHandValid(player.hand, gameRules))
            {
                // currentTurnPhase = TurnPhase::drawStep;
                currentTurnPhase = TurnPhase::mainPhase;
            }

            break;
        }
        case TurnPhase::mainPhase:
        {
            ExecuteChosenPlayerAction(player, currentTurnPhase, gameRules, gameStatus);

            break;
        }
        case TurnPhase::endTurnPhase:
        {
            // If both players passed without playing a card, Battle
            if (!HasACardBeenPlayedThisTurn(gameStatus) &&
                !HasACardBeenPlayedLastTurn(gameStatus))
            {
                currentTurnPhase = TurnPhase::battlePhase;

                break;
            }

            gameStatus.currentTurnOwner = player.id == 1 ? 2 : 1;
            gameStatus.turnsPlayed++;

            break;
        }
        case TurnPhase::battlePhase:
        {
            // Do Battle then immediately jump to the End Round Phase
            const int winnerId = CalculateRoundWinnerId(player, opponentPlayer);
            if (winnerId == 1) gameStatus.pointsPlayer1++;
            if (winnerId == 2) gameStatus.pointsPlayer2++;

            gameStatus.roundWinnerHistory.push_back(winnerId);

            gameStatus.roundsPlayed++;
            gameStatus.turnsPlayed++;

            //Clear the Fields
            player.cardsInPlayStack.clear();
            opponentPlayer.cardsInPlayStack.clear();

            currentTurnPhase = TurnPhase::endRoundPhase;
            break;
        }
        case TurnPhase::endRoundPhase:
        {
            // Stop executing turns if there is a winner.
            if (HasAPlayerWon(gameStatus, gameRules) != 0)
            {
                gameStatus.gameIsOver = true;
                return;
            }

            ExecuteChosenPlayerAction(player, currentTurnPhase, gameRules, gameStatus);

            break;
        }
    }
}

void ExecutePlayedCardEffects(const int cardID, Player &player, [[maybe_unused]] TurnPhase &turnPhase, [[maybe_unused]] const GameRules &gameRules, [[maybe_unused]] GameStatus &gameStatus)
{
    /*
     * In order to have a single source of truth, the cards database should have a column
     * that stats what behaviors that card has, in a specific order, in the form of an behaviorID.
     * This way, we only need to read the cards behaviorID to know what to do with it.
     * And if we change the card's text in the DB, we can update the behaviorID accordingly instead of hunting
     * down every instance of an if statement for that card's effects.
     *
     * On the other hand, cards will most likely all have very different and unique effects.
     * If I have to re-write a card's text I probably also have to re-write its implementation.
     */

    switch (cardID)
    {
        case 31: // "Draw Two" - Draw two cards.
        {
            DrawCardsFromDeckToHand(player, 2);
        }
        default:
        {
        }
    }
}

void DrawCardsFromDeckToHand(Player &player, const int amount)
{
    assert(amount <= static_cast<int>(player.deck.size())
        && "Trying to draw more cards than player has in its deck.\n");

    for (int i = 0; i < amount; ++i)
    {
        Card drawnCard = player.deck.at(0);
        drawnCard.rect.SetPosition(raylib::Vector2{-1000, -1000});
        drawnCard.faceUp = false;
        player.hand.push_back(drawnCard);
        player.deck.erase(player.deck.begin());
    }
}

void ExecuteChosenPlayerAction(Player &player, TurnPhase &turnPhase, const GameRules &gameRules, GameStatus &gameStatus)
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

    if (player.chosenAction.action != PlayerAction::null)
        LogAction(player, turnPhase, gameStatus);

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
        {
            //add hand to deck
            player.deck.insert(std::begin(player.deck), std::begin(player.hand), std::end(player.hand));
            player.hand.clear();

            //then shuffle deck

            std::ranges::shuffle(player.deck, HelperFunctions::GetRandomDevice());

            //then draw cards again
            DrawCardsFromDeckToHand(player, gameRules.initialHandSize);

            break;
        }
        case PlayerAction::shuffleHandIntoDeck:
        {
            break;
        }
        case PlayerAction::shuffleDeck:
        {
            break;
        }
        case PlayerAction::playCard:
        {
            //Copy the card from the hand to the Play field.
            for (size_t i = 0; i < player.hand.size(); ++i)
            {
                if (player.hand.at(i).uid != player.chosenAction.card.uid)
                    continue;

                const int cardID = player.hand.at(i).cardID;
                player.cardsInPlayStack.emplace_back(player.hand.at(i));
                //Then remove that card from the hand.
                player.hand.erase(player.hand.begin() + static_cast<long int>(i));

                ExecutePlayedCardEffects(cardID, player, turnPhase, gameRules, gameStatus);

                break;
            }

            PlaySound(GameSound::cardPlace01);

            break;
        }
        case PlayerAction::passTheTurn:
        {
            turnPhase = TurnPhase::endTurnPhase;

            break;
        }
        case PlayerAction::forfeit:
        {
            break;
        }
        case PlayerAction::finishRound:
        {
            if (gameStatus.pointsPlayer1 >= gameRules.pointsNeededToWin || gameStatus.pointsPlayer2 >= gameRules.pointsNeededToWin)
            {
                // The game ends and a winner is declared
            }

            turnPhase = TurnPhase::initialSetup;

            break;
        }
    }

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
        case PlayerAction::finishRound:
            return "Start New Round";
    }
    return "ERROR";
}

std::string TurnPhaseToString(const TurnPhase phase)
{
    switch (phase)
    {
        case TurnPhase::initialSetup:
            return "Initial Setup";
        case TurnPhase::mainPhase:
            return "Main Phase";
        case TurnPhase::endTurnPhase:
            return "End Phase";
        case TurnPhase::battlePhase:
            return "Battle Phase";
        case TurnPhase::endRoundPhase:
            return "End Round Phase";
    }
    return "null";
}
