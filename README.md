# Final-Project-ENGR103-Black-Jack
## Game Description 🎮

This is a simplified Blackjack-style card game built on the Adafruit Circuit Playground Express. The goal of this game is to simulate a real Blackjack game experience using buttons, switches, and motion inputs from the user, along with LED, audio, and Serial Monitor outputs. In this game, the player starts with $100 and places $10 bets each round until they either leave the table or run out of money.

## Game Rules 🃏
  - Both the player and the dealer are dealt 2 cards.
  - The dealer reveals one card, and both of the player’s cards are shown.
  - Then it becomes the player's turn (🟩LEDs turn green, signifying it's the player's turn), and they must decide between the following options: 
      - Hit – Press the left button on the board
	- The user will draw another card, which will be added to their total score
      - Stay – Press the right button on the board
	- The user will keep their score, and it will then be the dealer's turn
      - Double – Jerk the board directly outward away from yourself, moving it in the x plane.
	- You can only double if your starting hand equates to 9, 10, or 11. If you decide to double the user's bet of $10 will double to $20. Then the user will draw exactly one more card then it will move to the dealer's turn.
      - End Game – Slide the switch to stop the game and display your balance.
  - Card Values ♥️
    - 🂴 Number cards (2–10) are worth their face value. 
    - 🂾 Face cards (Jack, Queen, King) are each worth 10.
    - 🃁 Aces are worth 11 or 1, whichever benefits the hand more, which the game will decide for you.
  - Dealer Turn (🟥LEDs turn red, signifying it's the dealer's turn)
    - After the player finishes their turn, the dealer reveals their hidden card.
    - If the dealer has a total score less than 17, they continue to draw cards until they are greater than 17.
  - Outcome 🏆
    - The winner is the hand closest to 21 without going over.
    - If the player wins, they gain $10 and a happy jingle is played on the Arduino.
    - If the dealer wins, the player loses $10 and a sad jingle is played on the Arduino.
    - If it’s a tie, no money is exchanged.
  - What's Next? 🔁
    - The game repeats, and new cards are dealt to the dealer and player unless you leave the table or run out of money!

## Citations
  - For my motion recognition, I used Circuit Playground D6 Dice by Carter Nelson
      - Link: https://learn.adafruit.com/circuit-playground-d6-dice/shake-detect?gclid=CjwKCAiArY2fBhB9EiwAWqHK6ts2wIQm7kM5q3cekZq4pULfZ_c18fXmvtRhs3Ov0_reOrdmoGRfDhoCLAgQAvD_BwE
  - For my randomSeed code, I used a StackOverflow post titled: "Getting a truly random number in Arduino" to inspire what the motion detection for my random seed
      - Link: https://arduino.stackexchange.com/questions/50671/getting-a-truly-random-number-in-arduino

## Inputs:
| Input Type      | Function                      | Range/Type        |
| --------------- | ----------------------------- | ----------------- |
| Accelerometer X | Move board forward to DOUBLE  | Threshold: X < -30|
| Button Left     | HIT                           | Digital (falling) | 
| Button Right    | STAY                          | Digital (falling) |
| Switch          | END GAME                      | Digital (change)  |
## Outputs:
| Output Type    | Function                                                    |
| -------------- | ----------------------------------------------------------- |
| NeoPixels      | Shows whose turn it is. 🟥 Red for dealer, 🟩 Green for user |
| Speaker        | Win → play happy jingle, Lose → play sad jingle             |
| Serial Monitor | Display card values, account balance, dealer actions/player actions, and game result |



