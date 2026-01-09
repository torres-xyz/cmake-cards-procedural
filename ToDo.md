# To Do

- [ ] Add a delay to player 2's actions
- [ ] Announce round winner every round.
- [ ] Implement undoing playing a card before clicking end turn.
- [ ] Store a library of available cards and card lists better (currently they
  are just being declared in a vector<int> in constants.hpp).
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