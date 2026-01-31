# To Do

## Refactoring to Game Turn

- [ ] Add back the showing round winner scene.
    - Figure out a better way to display the overlay.
- [x] Implement mulligan.
    - [x] Create mulligan button.
- [x] Remove "Draw Step" from the phases. There is only the initial setup each turn, otherwise player's would never stop
  pumping their Units.
- [ ] Check all the TODOs before merging.

## Regular To Dos

- [ ] Move game logic to use a set of actions that each player can take.
- [ ] BUG: It is possible to start a Phase with no Units in the hand, bricking the game. (Make player lose the game?)
    - [ ] Add a log of each action.
- [ ] Redo player_test.cpp.
- [ ] Fix the way Action cards are displayed (remove the stats from the bottom).
- [ ] Add cards of different banners.
    - [ ] Create rule that you can only play Action cards on top of Units of the same Banner.
- [ ] Add current turn indicator.
- [ ] Implement undoing playing a card before clicking end turn.
- [ ] Animate opponent's cards being played.

## Lower Priority

- [ ] Analyse memory usage.

# Done

- [x] Give both players the same starting deck of cards.
- [x] Scene management.
- [x] Center text in rectangles.
- [x] Only reveal the cards after both players have played.
- [x] Start screen.
- [x] Announce winner when the game ends.
- [x] Switch to raylib-cpp for automatic memory management.
- [x] Integrate Dear Imgui for debugging purposes.
- [x] Button to restart the game when it ends.
- [x] Move cards with mouse.
    - [x] Create a Card Manager that deals with moving all the cards around.
- [x] Start adding Textures.
    - [x] TextureLoader
        - [x] Add ability to resize the textures being used in CardManager.
    - [x] Use textures also in the Field.
        - [x] The Field should not own/copy cards.
        - [x] Remove PlayField class.
    - [x] Add card back texture.
        - [x] Make player's card also be facing down when played.
- [x] Draw big card preview zone.
- [x] More forgiving card snapping zone.
- [x] Refactor scene handling so that each scene is a class.
- [x] Deck button to draw cards.
    - [x] 1 - Make players not draw their whole hands.
    - [x] 1.5 - Give buttons a deactivated state.
- [x] Introduce GamePhases states.
    - [x] Add a drawing step each turn.
    - [x] Remove turn owner variable, get this info from the Gameplay Phase.
    - [x] Dim player one play zone to indicate they have to draw first each turn.
- [x] Display Gameplay Phase on the screen.
- [x] Give buttons textures.
    - [x] Draw opponent's hand, face down.
- [x] Give both players shuffled decks.
- [x] Sound effects.
    - [x] Create Audio Manager so that we don't load the same sounds.
    - [x] Button presses.
    - [x] Background music.
    - [x] Playing cards.
- [x] Display other player's hand, face down.
- [x] Load assets on demand.
- [x] Unit testing framework.
- [x] Create new scene to display match winner.
- [x] Custom font (Aoboshi One, perhaps).
- [x] Make cards in hand bigger when hovered.
- [x] Remove preview zone.
- [x] Expand playfield and make cards there bigger.
- [x] Make text in cards more legible.
- [x] Implement a more complex card game.
    - [x] Add cards that can boost the stats of the played card.
        - [x] Disallow playing non-unit cards as the main card.
        - [x] Resolve the Action cards effects when the turn ends.
        - [x] Add Action-type of cards.
    - [x] Add multiple stages to a turn, so that players can play more action cards after their opponent has played
      action cards.
    - [x] Add pass turn button.
    - [x] Randomly generate cards with different stats.
    - [x] Make the cards in the hand zone larger.
    - [x] Make the battle winner the owner of the card with the higher overall stats.
- [x] Remove RPS card types.
- [x] Add a stats total to the UI.
- [x] Make sure initial hand is always valid.
- [x] Add hovering to the cards in the play zone so we can see all cards in the stacks.
    - [x] Use UIDs to identify which card is being hovered.
    - [x] Center the zoomed in card around its original position.
- [x] Make the game a best of 3.
- [x] Add a delay to player 2's actions.
- [x] Announce round winner every round.
- [x] Make GetCardArtTexture retrieve the path from the csv file.
- [x] Create a fixed card library (no more random cards).
    - [x] At least 30 different cards.
    - [x] Add card art for each card.
    - [x] Create a NameToID() function.
    - [x] Make 2 different decks with these cards, one for each player.
    - [x] Create tests for the new functions.
        - [x] Test if all the cards in the DB have different ids.
        - [x] Test if all the textures in the DB exist and can be loaded successfully.
        - [x] Test all cards created have a unique uid;
