#include "game_play_phases.hpp"
#include <cassert>
#include <iostream>

#include "audio.hpp"
#include "constants.hpp"
#include "raylib-cpp.hpp"
#include "player.hpp"

void UpdateGameplayPhases(GameplayPhase &currentPhase, Player &player1, Player &player2,
                          const int goingFirst, std::random_device &rd)
{
    static float timeSinceStartOfPhase{};
    static constexpr float playerActionWaitTime{1};
    static int playerOnePreviousCardsPlayed{};
    static int playerTwoPreviousCardsPlayed{};
    static bool playerOneHasPlayedOnThePreviousTurn{};
    static bool playerTwoHasPlayedOnThePreviousTurn{};

    auto ChangePhase
    {
        [&currentPhase](const GameplayPhase nextPhase) {
            currentPhase = nextPhase;
            timeSinceStartOfPhase = 0;
        }
    };

    auto HasPlayerPlayed
    {
        [](const Player &player) {
            if (player.id == 1 && playerOnePreviousCardsPlayed < player.cardsPlayed)
            {
                return true;
            }
            if (player.id == 2 && playerTwoPreviousCardsPlayed < player.cardsPlayed)
            {
                return true;
            }
            return false;
        }
    };


    timeSinceStartOfPhase += GetFrameTime();

    switch (currentPhase)
    {
        case GameplayPhase::uninitialized:
        {
            //Reset everything gameplay wise
            InitializePlayerWithAdvancedDeck(player1, rd);
            InitializePlayerWithAdvancedDeck(player2, rd);

            ChangePhase(GameplayPhase::initialHandDraw);

            break;
        }
        case GameplayPhase::initialHandDraw:
        {
            player1.cardsInPlayStack.clear();
            player2.cardsInPlayStack.clear();

            player1.hasDrawnThisTurn = false;

            //Auto draw initial hand for P2
            if (player2.hand.size() != constants::initialHandSize)
            {
                DrawCardsFromDeckToHand(player2, constants::initialHandSize);
                break;
            }
            //Wait for P1 to click the draw button enough times
            if (player1.hand.size() != constants::initialHandSize)
            {
                break;
            }
            if (goingFirst == 1)
            {
                ChangePhase(GameplayPhase::playerOnePlaying);
                break;
            }
            if (goingFirst == 2)
            {
                ChangePhase(GameplayPhase::playerTwoPlaying);
                break;
            }

            break;
        }
        case GameplayPhase::playerOneDrawing:
        {
            //if player's deck is empty, skip drawing phase
            if (player1.deck.empty())
            {
                ChangePhase(GameplayPhase::playerOnePlaying);
                break;
            }

            //Wait for Player 1 to draw a card (by clicking the deck button).
            if (player1.hasDrawnThisTurn)
            {
                player1.hasDrawnThisTurn = false;
                ChangePhase(GameplayPhase::playerOnePlaying);
                break;
            }
            break;
        }
        case GameplayPhase::playerOnePlaying:
        {
            //reset that this player has played a card on the previous turn.
            playerOneHasPlayedOnThePreviousTurn = false;
            //Check if the opponent has played a card on the previous turn
            if (player2.cardsPlayed > playerTwoPreviousCardsPlayed)
            {
                playerTwoHasPlayedOnThePreviousTurn = true;
                playerTwoPreviousCardsPlayed = player2.cardsPlayed;
            }

            //Wait until player 1 has played
            if (!HasPlayerPlayed(player1))
            {
                break;
            }
            //Wait until player 1 has pressed the end turn button
            if (!player1.hasEndedTheTurn)
            {
                break;
            }

            //If player 2 has also played we battle, otherwise it must be player 2's turn next.
            // if (HaveBothPlayersPlayed())
            // {
            //     ChangePhase(GameplayPhase::battle);
            // }
            else if (!HasPlayerPlayed(player2))
            {
                ChangePhase(GameplayPhase::playerTwoDrawing);
            }
            break;
        }
        case GameplayPhase::playerTwoDrawing:
        {
            //if player's deck is empty, skip drawing phase
            if (player2.deck.empty())
            {
                ChangePhase(GameplayPhase::playerTwoPlaying);
                break;
            }
            //Player 2 draws automatically
            DrawCardsFromDeckToHand(player2, 1);

            ChangePhase(GameplayPhase::playerTwoPlaying);
            break;
        }
        case GameplayPhase::playerTwoPlaying:
        {
            if (timeSinceStartOfPhase < playerActionWaitTime) break;

            //reset that this player has played a card on the previous turn.
            playerTwoHasPlayedOnThePreviousTurn = false;
            //Check if the opponent has played a card on the previous turn
            if (player1.cardsPlayed > playerOnePreviousCardsPlayed)
            {
                playerOneHasPlayedOnThePreviousTurn = true;
                playerOnePreviousCardsPlayed = player1.cardsPlayed;
            }


            //Player 2 plays a card, always index 0 for now
            player2.heldCardIndex = 0;
            PutCardInPlay(player2);

            playerOnePreviousCardsPlayed = player1.cardsPlayed;
            // if (HaveBothPlayersPlayed())
            // {
            //     ChangePhase(GameplayPhase::battle);
            // }
            // else
            {
                ChangePhase(GameplayPhase::playerOneDrawing);
            }
            break;
        }
        case GameplayPhase::battle:
        {
            // Wait a bit before clearing the field
            if (timeSinceStartOfPhase < playerActionWaitTime)
                break;

            const int turnWinner = BattleCards(player1.cardsInPlayStack, player2.cardsInPlayStack);
            if (turnWinner == 1)
            {
                player1.score++;
            }
            if (turnWinner == 2)
            {
                player2.score++;
            }

            //Remove the played cards
            player1.cardsInPlayStack.clear();
            player2.cardsInPlayStack.clear();
            player1.hasDrawnThisTurn = false;
            player1.hasEndedTheTurn = false;

            if (player1.hand.empty() && player1.deck.empty() &&
                player2.hand.empty() && player2.deck.empty())
            {
                ChangePhase(GameplayPhase::gameOver);
                break;
            }

            //After the battle, give the turn to the player who went first
            if (goingFirst == 1)
            {
                ChangePhase(GameplayPhase::playerOneDrawing);
                break;
            }
            if (goingFirst == 2)
            {
                ChangePhase(GameplayPhase::playerTwoDrawing);
                break;
            }
            break;
        }
        case GameplayPhase::gameOver:
        {
            break;
        }
    }
}

void DrawCardsFromDeckToHand(Player &player, const int amount)
{
    assert(amount <= static_cast<int>(player.deck.size())
        && "Trying to draw more cards than player has in its deck.\n");

    for (int i = 0; i < amount; ++i)
    {
        Card drawnCard = player.deck.back();
        drawnCard.rect.SetPosition(raylib::Vector2{-1000, -1000});
        drawnCard.faceUp = false;
        player.hand.push_back(drawnCard);
        player.deck.pop_back();
    }
}

void PutCardInPlay(Player &player)
{
    assert(player.heldCardIndex > -1 && "Trying to play a card of negative index.");

    //Can only play Non-Unit cards after a Unit card is already in play
    // if (player.hand.at(static_cast<size_t>(player.heldCardIndex)).type == CardType::action) return;

    player.cardsPlayed++;

    //Copy the card from the hand to the Play field.
    player.cardsInPlayStack.emplace_back(player.hand.at(static_cast<size_t>(player.heldCardIndex)));
    player.cardsInPlayStack.at(0).faceUp = true;

    if (player.id == 1)
    {
        player.cardsInPlayStack.at(0).rect = raylib::Rectangle{
            constants::playerOnePlayfieldCardZoneRect.x,
            constants::playerOnePlayfieldCardZoneRect.y,
            constants::cardWidth,
            constants::cardHeight
        };
    }
    else if (player.id == 2)
    {
        player.cardsInPlayStack.at(0).rect = raylib::Rectangle{
            constants::playerTwoPlayfieldCardZoneRect.x,
            constants::playerTwoPlayfieldCardZoneRect.y,
            constants::cardWidth,
            constants::cardHeight
        };
    }

    //Then remove that card from the hand.
    player.hand.erase(player.hand.begin() + player.heldCardIndex);
    //The player is now not holding any card.
    player.heldCardIndex = -1;

    //Play sound
    PlaySound(GameSound::cardPlace01);
}

int BattleCards(const std::vector<Card> &cardStack1, const std::vector<Card> &cardStack2)
{
    const Card &card1 = cardStack1.at(0);
    int highestStatCard1{0};
    if (card1.body > highestStatCard1) highestStatCard1 = card1.body;
    if (card1.mind > highestStatCard1) highestStatCard1 = card1.mind;
    if (card1.soul > highestStatCard1) highestStatCard1 = card1.soul;

    const Card &card2 = cardStack2.at(0);
    int highestStatCard2{0};
    if (card2.body > highestStatCard2) highestStatCard2 = card2.body;
    if (card2.mind > highestStatCard2) highestStatCard2 = card2.mind;
    if (card2.soul > highestStatCard2) highestStatCard2 = card2.soul;

    if (highestStatCard1 > highestStatCard2)
    {
        return 1;
    }
    if (highestStatCard2 > highestStatCard1)
    {
        return 2;
    }
    //Draw
    return 0;
}

std::string GameplayPhaseToString(const GameplayPhase phase)
{
    switch (phase)
    {
        case GameplayPhase::uninitialized:
            return "Uninitialized";
        case GameplayPhase::initialHandDraw:
            return "Initial Hand Draw";
        case GameplayPhase::playerOneDrawing:
            return "Player One Drawing";
        case GameplayPhase::playerOnePlaying:
            return "Player One Playing";
        case GameplayPhase::playerTwoDrawing:
            return "Player Two Drawing";
        case GameplayPhase::playerTwoPlaying:
            return "Player Two Playing";
        case GameplayPhase::battle:
            return "Battle";
        case GameplayPhase::gameOver:
            return "Game Over";
    }
    return "Error";
}


