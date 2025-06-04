#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

const int switchPin =7;
const int LeftButton = 4;
const int RightButton = 5; 

AsyncDelay gameTimer;
const int gameTime = 5000;

volatile bool leftButtonFlag = false;
volatile bool rightButtonFlag = false;
volatile bool switchFlag = false;

String suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
String faceVals[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
int cardVals[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

String cards[4][13][13];

int gameRound = 0;

int accountBalance = 100;
int playerTotal = 0;
int dealerTotal = 0;

float X, Y, Z;

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
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);
  

  pinMode(switchPin, INPUT_PULLUP);  // Switch pin with pullup enabled
  pinMode(LeftButton, INPUT_PULLDOWN); // Left button, pulldown enabled
  pinMode(RightButton, INPUT_PULLDOWN); // Right button, pulldown enabled

  attachInterrupt(digitalPinToInterrupt(switchPin), handleSwitchChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LeftButton), handleButtonChangeLeft, FALLING);
  attachInterrupt(digitalPinToInterrupt(RightButton), handleButtonChangeRight, FALLING);

  gameTimer.start(gameTime, AsyncDelay::MILLIS);

  delay(2000);
}

void loop() {
  
  // Accelerometer, Shake Detection EXAMPLE:
  X = 0;
  Y = 0;
  Z = 0;
  for (int i=0; i<10; i++) {
    X += CircuitPlayground.motionX();
    Y += CircuitPlayground.motionY();
    Z += CircuitPlayground.motionZ();
    delay(1);
  }
  X /= 10;
  Y /= 10;
  Z /= 10;

  if (gameRound == 0){
    FirstCards();
    gameRound++;
  }


  if (X<-30){
    Serial.println(X);
    Serial.println("You chose to DOUBLE");

  }

  if (Z>30){
    Serial.println(Y);
    Serial.println("You chose to SPLIT");
  }

  if (leftButtonFlag){
    Serial.println("You chose to HIT");
    playerTurn();
    leftButtonFlag = false;
    gameRound++;
  }

  if (rightButtonFlag){
    Serial.println("You chose to STAY");
    Serial.print("Total: ");
    Serial.println(playerTotal);
    rightButtonFlag = false;
    gameRound++;
    
  }


  // If switch is flicked
  if (switchFlag){
    Serial.print("You left the table with: $");
    Serial.print(accountBalance);
    switchFlag = false;
  }
  


  if (gameTimer.isExpired()){
    gameTimer.repeat();
  }
  delay(100);
}

void dealerTurn(){
  int dealerVal = drawCard();
  dealerTotal += dealerVal;
  if (dealerTotal > 21){
    Serial.println("Dealer bust!");
    accountBalance += 10;
    delay(3000);
    dealerTotal = 0;
    gameRound = 0;
  }

  else if (dealerTotal < 17){
    dealerVal = drawCard();
  }

  else{
    Serial.print("Dealer score right now is: ");
    Serial.println(dealerTotal);
  }
}

void playerTurn(){
  int val = drawCard();
  playerTotal += val;
  if (playerTotal > 21){
    Serial.println("You bust!");
    accountBalance -= 10;
    delay(3000);
    playerTotal = 0;
    gameRound = 0;
  }

  else{
    Serial.print("Your total score right now is: ");
    Serial.println(playerTotal);
  }

}

int drawCard(){

  int randomSuit = random(0, 4);
  int randomVal = random(0, 13);
  
  Serial.print("You drew the ");
  Serial.print(faceVals[randomVal]);
  Serial.print(" of ");
  Serial.println(suits[randomSuit]);

  return cardVals[randomVal];
}

void FirstCards(){
  // Run player logic
  int playerFirstCard = drawCard();
  int playerSecondCard = drawCard();
  playerTotal = playerFirstCard + playerSecondCard;
  Serial.print("Your starting total: ");
  Serial.println(playerTotal);

  // Create Dealer Logic
  int dealerFirstCard = drawCard();
  int dealerSecondCard = drawCard();
  dealerTotal = dealerFirstCard + dealerSecondCard;
  Serial.print("Dealer starting total: ");
  Serial.println(dealerTotal);
  }

