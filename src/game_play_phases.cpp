#include "game_play_phases.hpp"
#include <cassert>
#include <iostream>

#include "audio.hpp"
#include "constants.hpp"
#include "raylib-cpp.hpp"
#include "player.hpp"

void UpdateGameplayPhases(GameplayPhase &currentPhase, Player &player1, Player &player2,
                          const int goingFirst, bool &player1HasDrawnThisTurn, std::random_device &rd)
{
    static float timeSinceStartOfPhase{};
    static constexpr float playerActionWaitTime{1};

    auto ChangePhase
    {
        [&currentPhase](const GameplayPhase nextPhase) {
            currentPhase = nextPhase;
            timeSinceStartOfPhase = 0;
        }
    };
    auto HaveBothPlayersPlayed
    {
        [player1, player2]() {
            return player1.cardInPlay.type != CardType::invalid &&
                   player2.cardInPlay.type != CardType::invalid;
        }
    };
    auto HasPlayerPlayed
    {
        [](const Player &player) {
            return player.cardInPlay.type != CardType::invalid;
        }
    };

    timeSinceStartOfPhase += GetFrameTime();

    switch (currentPhase)
    {
        case GameplayPhase::uninitialized:
        {
            //Reset everything gameplay wise
            InitializePlayerWithDeck(player1, rd);
            InitializePlayerWithDeck(player2, rd);

            ChangePhase(GameplayPhase::initialHandDraw);

            break;
        }
        case GameplayPhase::initialHandDraw:
        {
            player1.cardInPlay.type = CardType::invalid;
            player2.cardInPlay.type = CardType::invalid;
            player1HasDrawnThisTurn = false;

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
            if (player1HasDrawnThisTurn)
            {
                player1HasDrawnThisTurn = false;
                ChangePhase(GameplayPhase::playerOnePlaying);
                break;
            }
            break;
        }
        case GameplayPhase::playerOnePlaying:
        {
            //Wait until player 1 has played
            if (!HasPlayerPlayed(player1))
            {
                break;
            }

            //If player 2 has also played we battle, otherwise it must be player 2's turn next.
            if (HaveBothPlayersPlayed())
            {
                ChangePhase(GameplayPhase::battle);
            }
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

            //Player 2 plays a card, always index 0 for now
            player2.heldCardIndex = 0;
            PutCardInPlay(player2);

            if (HaveBothPlayersPlayed())
            {
                ChangePhase(GameplayPhase::battle);
            }
            else
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

            if (player1.cardInPlay.faceUp == false ||
                player2.cardInPlay.faceUp == false)
            {
                //Play sound
                PlaySound(GameSound::cardPlace01);
                //reveal played cards
                player1.cardInPlay.faceUp = true;
                player2.cardInPlay.faceUp = true;
            }

            //Wait a bit more after revealing the cards
            if (timeSinceStartOfPhase < playerActionWaitTime + 1)
                break;

            const int turnWinner = BattleCards(player1.cardInPlay, player2.cardInPlay);
            if (turnWinner == 1)
            {
                player1.score++;
            }
            if (turnWinner == 2)
            {
                player2.score++;
            }

            //Remove the played cards
            player1.cardInPlay.type = CardType::invalid;
            player2.cardInPlay.type = CardType::invalid;
            player1HasDrawnThisTurn = false;

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

    //Copy the card from the hand to the inPlay field.
    player.cardInPlay = player.hand.at(static_cast<size_t>(player.heldCardIndex));
    player.cardInPlay.faceUp = false;

    if (player.id == 1)
    {
        player.cardInPlay.rect = raylib::Rectangle{
            constants::playerOnePlayfieldCardZoneRect.x,
            constants::playerOnePlayfieldCardZoneRect.y,
            constants::cardWidth,
            constants::cardHeight
        };
    }
    else if (player.id == 2)
    {
        player.cardInPlay.rect = raylib::Rectangle{
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

int BattleCards(const Card &card1, const Card &card2)
{
    const CardType typeCard1 = card1.type;
    const CardType typeCard2 = card2.type;

    if (typeCard1 == CardType::invalid || typeCard2 == CardType::invalid)
    {
        std::cerr << "Invalid card" << "\n";
        return -1;
    }

    if (typeCard1 == typeCard2) return 0; //draw
    if (typeCard1 == CardType::paper)
    {
        if (typeCard2 == CardType::rock) return 1;
        if (typeCard2 == CardType::scissors) return 2;
    }
    if (typeCard1 == CardType::rock)
    {
        if (typeCard2 == CardType::scissors) return 1;
        if (typeCard2 == CardType::paper) return 2;
    }
    if (typeCard1 == CardType::scissors)
    {
        if (typeCard2 == CardType::paper) return 1;
        if (typeCard2 == CardType::rock) return 2;
    }
    std::cerr << "Something went wrong with the Rock-Paper-Scissors logic." << "\n";
    return -1;
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


