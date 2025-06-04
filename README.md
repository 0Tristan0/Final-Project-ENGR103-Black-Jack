# Final-Project-ENGR103-Black-Jack
## Game Description 🎮
This is a simplified Blackjack-style casino game built on the Adafruit Circuit Playground Express (CPX). The goal is to simulate a playable round-based Blackjack experience with physical inputs and clear visual/audio outputs. The player starts with $100, placing $10 bets each round, and plays until they either quit using the switch or run out of money.

## Game Rules 🃏
  - Both the player and the dealer are dealt 2 cards.
  - The dealer reveals one card, and both of the player’s cards are shown.
  - The player chooses one of the following actions:
      - Hit – Tilt the board forward/up/down to draw another card.
      - Stay – Tilt the board left or right to keep your current hand.
      - Double – Press either button (A or B) to double your bet, draw one more card, then end your turn.
      - Split – Press both buttons at the same time. If your first two cards are of the same value, they are split into two separate hands.
      - End Game – Slide the switch to stop the game and display your balance.
  - Card Values ♥️
    - 🂴 Number cards (2–10) are worth their face value. 
    - 🂾 Face cards (Jack, Queen, King) are each worth 10.
    - 🃁 Aces are worth 11 or 1, whichever benefits the hand more.
  - Dealer Turn
    - After the player finishes their turn, the dealer reveals their hidden card.
    - The dealer draws cards (hits) until their total is 17 or more.
  - Outcome 🏆
    - The winner is the hand closest to 21 without going over.
    - If the player wins, they gain $10.
    - If the dealer wins, the player loses $10.
    - If it’s a tie (push), no money is exchanged.
## Inputs:
| Input Type      | Function                      | Range/Type        | Mapping                                   |
| --------------- | ----------------------------- | ----------------- | ----------------------------------------- |
| Accelerometer Z | Move board up/down = DOUBLE   | 0–1000            | Map to **orange** color on NeoPixels      |
| Accelerometer X | Move board left/right = SPLIT | 0–1000            | Map to **blue** color on NeoPixels        |
| Button Left     | HIT                           | Digital (falling) | No mapping needed; draws another card     |
| Button Right    | STAY                          | Digital (falling) | No mapping needed; ends player’s turn     |
| Switch          | END GAME                      | Digital (change)  | No mapping; exits game and prints balance |
## Outputs:
| Output Type    | Function                                                    |
| -------------- | ----------------------------------------------------------- |
| NeoPixels      | Show action states: green (HIT), red (STAY), etc.           |
| Speaker        | Win → play jingle, Lose → play sad sound                    |
| Serial Monitor | Display card values, money, dealer actions, and game result |



