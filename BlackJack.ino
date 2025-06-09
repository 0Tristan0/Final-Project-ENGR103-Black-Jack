// Library that allows us to use Arduino hardware features
#include <Adafruit_CircuitPlayground.h>

// Pin assignments for switches and buttons
const int switchPin =7;
const int LeftButton = 4;
const int RightButton = 5; 

// Creating flags for inputs 
volatile bool leftButtonFlag = false;
volatile bool rightButtonFlag = false;
volatile bool switchFlag = false;

// These are arrays representing the card data. 
// Suits represents the suits of the cards 
String suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
// faceVals has all the names of the faces of the cards
String faceVals[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
// cardVals holds all of the actual values of each of the cards
int cardVals[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

// These are my game variables
// This represents if I am starting the game or not, if so, then I deal 2 cards to each player
bool gameStart = true;

// accountBalance holds the balance of the player's account 
int accountBalance = 100;

// These hold the total values of all of their cards combined
int playerTotal = 0;
int dealerTotal = 0;

// This is the dealer's first and second card, which the second card is revealed after the player's turn is over, so it must be a global variable to be accessed in multiple functions easily.
int dealerFirstCard;
int dealerSecondCard;

// This variable is true when we want to display the card that is drawn by the dealer to the user, and false when it should not be shown to the user
bool display = true;

// This variable stores the bet for the round 
int bet;

// Accelerometer values
float X, Y, Z;

// Inturrupt Handlers
void handleSwitchChange(){
  switchFlag = true;
}

void handleButtonChangeRight(){
  rightButtonFlag = true;
}

void handleButtonChangeLeft(){
  leftButtonFlag = true;
}


void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  // Part of the example code that configures how the LIS3DH accelerometer measures acceleration in the range of: ±8G, meaning up to 8 times the force of gravity
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);

  pinMode(switchPin, INPUT_PULLUP);  // Switch pin with pullup enabled, meaning it assumes HIGH when off and LOW when on
  pinMode(LeftButton, INPUT_PULLDOWN); // Left button, pulldown enabled, meaning it assumes the button is LOW when off and HIGH when on
  pinMode(RightButton, INPUT_PULLDOWN); // Right button, pulldown enabled

  // Set up ISRs
  attachInterrupt(digitalPinToInterrupt(switchPin), handleSwitchChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LeftButton), handleButtonChangeLeft, FALLING);
  attachInterrupt(digitalPinToInterrupt(RightButton), handleButtonChangeRight, FALLING);

  // Arduino's random function starts at the same spot no matter what, unless you change its starting position, also known as a seed
  // Here we are capturing the motion in all directions and then turning them into huge numbers so even the smallest movements get recorded
  // The random seed starts the random function at a place decided by the movement, so the board is truly random
  // You can use anything as you see, such as an analog read or a sound read, but I chose to use motion
  // I used a Stack Overflow post title: "Getting a truly random number in Arduino" to help me construct this code
  // link: https://arduino.stackexchange.com/questions/50671/getting-a-truly-random-number-in-arduino
  int seed = (int)(CircuitPlayground.motionX() * 1000 + CircuitPlayground.motionY() * 1000 + CircuitPlayground.motionZ() * 1000);
  randomSeed(seed);

  delay(2000);
}

void loop() {
  
  // Set bet equal to ten at the start of each round 
  bet = 10;

  // Set the pixel to green, indicating it is the player's turn 
  for(int i=0; i<10; i++){
    CircuitPlayground.setPixelColor(i, 0, 255, 0);
    }

  // Accelerometer, Shake Detection EXAMPLE: Circuit Playground D6 Dice by Carter Nelson 
  //link: https://learn.adafruit.com/circuit-playground-d6-dice/shake-detect?gclid=CjwKCAiArY2fBhB9EiwAWqHK6ts2wIQm7kM5q3cekZq4pULfZ_c18fXmvtRhs3Ov0_reOrdmoGRfDhoCLAgQAvD_BwE 
  X = 0;
  Y = 0;
  Z = 0;
  for (int i=0; i<10; i++) {
    // Get the motion
    X += CircuitPlayground.motionX();
    Y += CircuitPlayground.motionY();
    Z += CircuitPlayground.motionZ();
    delay(1);
  }
  X /= 10;
  Y /= 10;
  Z /= 10;

  // If the user has no money left, they have to leave the table
  if (accountBalance <= 0) {
    Serial.println("\n----------YOU HAVE NO MONEY LEFT---------\n");
    loseJingle();
    delay(50000000); // Puase until they quit the program 
  }


  // If it is the beginning of the game, run the FirstCards function to give both the dealer and the user their first hands
  if (gameStart){
    FirstCards();
    gameStart = false;
  }

  // This is for if the user wants to double, which can only happen if their total is 9,10, or 11 based on their first hand 
  if (X<-30 && (playerTotal == 9 || playerTotal == 10 || playerTotal == 11)){
    Serial.println("You chose to DOUBLE");
    // double the initial 10-dollar bet
    bet += 10;
    Serial.print("Bet: ");
    Serial.println(bet);

    // then hit the player one more time
    Serial.print("You drew: ");
    playerHit();
    dealerTurn(); // stright to dealer turn

  }
  
  // If the user decides to HIT
  if (leftButtonFlag){
    Serial.println("\nYou chose to HIT");
    Serial.print("You drew: ");
    playerHit();
    leftButtonFlag = false; // reset the left button flag back to false
  }

  // if the user decides to stay
  if (rightButtonFlag){
    Serial.println("\nYou chose to STAY");
    // tell the user their score
    Serial.print("Your total score is: ");
    Serial.println(playerTotal);
    rightButtonFlag = false; // reset the right button flag back to false
    dealerTurn(); // run the dealer turn function
  }


  // If the switch is flicked,
  if (switchFlag){
    Serial.print("\nYou left the table with: $");
    Serial.print(accountBalance);
    Serial.println("");
    // Set LEDs to blue to show the user has left the table.
    for(int i=0; i<10; i++){
      CircuitPlayground.setPixelColor(i, 0, 0, 255);
    }

    // Wait until the user joins back into the table by flicking the switch
    while(digitalRead(switchPin) == LOW){
      delay(10); // WAIT
    }
    gameStart = true; // Restart the game and deal out cards
    switchFlag = false; // reset the flag
  }
}

void dealerTurn(){
  // Set the pixils to RED, indicating that it is the dealer's turn 
  for(int i=0; i<10; i++){
    CircuitPlayground.setPixelColor(i, 255, 0, 0);
    }
  Serial.println("\n----------Dealers Turn---------\n");
  delay(3000);
  Serial.print("The dealers second card was ");
  Serial.println(dealerSecondCard);
  Serial.print("So their total is: ");
  Serial.println(dealerTotal);

  // Dealer continues to hit until they are over 17
  while (dealerTotal < 17){
    Serial.println("\nDealer must draw again because they are less than 17");
    Serial.print("Dealer drew ");
    // Make the dealer draw another card and add to the total score
    int dealerVal = drawCard();
    dealerTotal += dealerVal;
    delay(3000);

    // If the dealer draws an Ace and it causes them to bust it resets it to a one
    if (dealerVal > 11 && dealerTotal > 21){
      // Use Ace as a one
      dealerTotal -= 10;
    }
  }

  Serial.print("\nDealer score right now is: ");
  Serial.println(dealerTotal);

  // If the dealer's score is less than the player's score, then they lose
  if (dealerTotal < playerTotal ){
    delay(3000);
    Serial.println("\n----------You Win----------\n");
    winJingle(); // play win jingle
    accountBalance += bet; // Add the bet to the balance
    Serial.print("Account Balance: $");
    Serial.println(accountBalance);
    gameStart = true; // restart game
  }

  // If the dealer busts, they lose
  else if (dealerTotal > 21){
    Serial.println("\nDealer bust!");
    delay(3000);
    Serial.println("\n----------You Win----------\n");
    winJingle(); 
    accountBalance += bet;
    Serial.print("Account Balance: $");
    Serial.println(accountBalance);
    gameStart = true;
  }

  // In the event of a tie dont nobody loses money, so the game is now reset
  else if (dealerTotal == playerTotal){
    delay(3000);
    Serial.println("\n----------TIE----------\n");
    Serial.print("Account Balance: $");
    Serial.println(accountBalance);
    gameStart = true;
  }
  // If the dealer's score is higher than the player's score, then they win and the player loses
  else{
    delay(3000);
    Serial.println("\n----------You Lost----------\n");
    loseJingle();
    accountBalance -= bet;
    Serial.print("Account Balance: $");
    Serial.println(accountBalance);
    gameStart = true;
  }
}

// Function for if the player decides to hit based on their hand
void playerHit(){
  int val = drawCard(); // val is the valye retured by the functiond drawCard() with is tghe value of a random card
  playerTotal += val; // add val to the user total because they decided to hit

  // If the user drew an Ace and it causes them to bust, turn it into a one
  if (val == 11){
    if (playerTotal > 21){
      // Turn the ace into a one when it causes the user to bust
      playerTotal -= 10;
    }
    Serial.print("\nYour total score is: ");
    Serial.println(playerTotal);
  }

  else if (playerTotal > 21){ // if you bust
    Serial.println("\nYou bust!");
    Serial.println("\n----------You Lost----------\n");
    loseJingle();
    accountBalance -= bet;
    Serial.print("Account Balance: $");
    Serial.println(accountBalance);
    gameStart = true;
  }

  // if the player does not bust, print their total score
  else{
    Serial.print("\nYour total score is: ");
    Serial.println(playerTotal);
  }

}

int drawCard(){
  // display is the variable that signifies whether or not we want to print the value of the card out to the user
  if (display){
    // a random suit and card value
    int randomSuit = random(0, 4);
    int randomVal = random(0, 13);
    
    // print that inforamtion
    Serial.print("the ");
    Serial.print(faceVals[randomVal]);
    Serial.print(" of ");
    Serial.print(suits[randomSuit]);

    // return the value of the cards using the index of the randomVal, as it is the same as the card value
    return cardVals[randomVal];
  }
  // Do the same as above, but do not print anything out
  else{
    int randomSuit = random(0, 4);
    int randomVal = random(0, 13);
    return cardVals[randomVal];
  }
  
}

void FirstCards(){

  // Welcome text
  Serial.println(" -----------------------------");
  Serial.println(" |                           |");
  Serial.println(" |   Welcome to Black Jack   |");
  Serial.println(" |                           |");
  Serial.println(" -----------------------------");
  Serial.println("\nLets start by giving you your fist hand!\n");

  
  // First, we want to tell the user which 2 cards they drew.
  Serial.print("You drew : ");
  int playerFirstCard = drawCard(); // run the draw card function to get their first card
  Serial.print(" and ");
  int playerSecondCard = drawCard(); // run the draw card function to get their second card

  // Get the player's total by adding together the player's first and second card values
  playerTotal = playerFirstCard + playerSecondCard;

  // Print to the user what their starting total is
  Serial.print("\nYour starting total: ");
  Serial.println(playerTotal);



  // CNow we want to tell the user what the dealer's first card is, but not the second
  Serial.print("\nThe dealer drew : ");
  dealerFirstCard = drawCard();

  // When we set display equal to false in the drawCard() function, we still get another card, but we do not print the values of that card
  display = false;
  dealerSecondCard = drawCard();
  display = true; // set back equal to true so that way if the user draws more cards it prints what they are.

  //Calculate the total score of the dealer 
  dealerTotal = dealerFirstCard + dealerSecondCard;

  // Tell the user it is now their turn to decide what they want to do
  Serial.println("\n----------Your Turn---------");
  }

// This is a function that plays the happy jingle when the user wins a round
  void winJingle(){
    CircuitPlayground.playTone(262, 150);// C4
    CircuitPlayground.playTone(330, 150);// E4
    CircuitPlayground.playTone(392, 150);// G4
    CircuitPlayground.playTone(523, 300);// C5
  }

// This is a function that plays the sad jingle when the user loses a round
  void loseJingle() {
    CircuitPlayground.playTone(196, 400);// G3
    CircuitPlayground.playTone(185, 400);// F#3
    CircuitPlayground.playTone(174, 400);// F3
    CircuitPlayground.playTone(165, 800);// E3
}
