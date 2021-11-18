// Name: Ayalew Wolde
// Instructor: Dr. Fatma Serce 
// CS 351 - Computer Architecture
// Assignment 4 - Table Tennis Scoreboard

// includes the LiquidCrystal library for the LCD
#include <LiquidCrystal.h>

// Setting up constants for the pin numbers.

// Sets pins for 4 buttons.
const int buttOne = 10;
const int buttTwo = 9;
const int buttThree = 8;
const int buttFour = 7;

// Sets LED pins.
const int leftLed = 12;
const int rightLed = 13;

//Setting up LCD pins
LiquidCrystal lcd(6, 5, 4, 3, 2, 1);

// tempA and tempB 
// set points for player A and B
int setPointA = 0;
int setPointB = 0;
// game points for player A and B
int gamePointA = 0;
int gamePointB = 0;

//for keeping temporary scores in case of decrementing
int tempA;
int tempB;
String tempLast;
// We have 2 serves per side until 10 points is reached
int serves = 0;// keep track of how many serves per side
               // 2 serves per side unitl 10 points reachd then 1 serve perside
// For swapping names/sides.
String A = "A";
String B = "B";

void setup() {
  // informs the library of LCD Size
  lcd.begin(16, 2);

  // Setting the initial scores of zero.
  lcd.setCursor(0, 0);
  lcd.print("A  0");
  lcd.setCursor(7, 0);
  lcd.print(": B  0");
  lcd.setCursor(0, 1);
  lcd.print("SET 0  : SET 0");

  // Set up the pushbutton pins to be an input:
  pinMode(buttOne, INPUT_PULLUP);
  pinMode(buttTwo, INPUT_PULLUP);
  pinMode(buttThree, INPUT_PULLUP);
  pinMode(buttFour, INPUT_PULLUP);

  // Set up the LED pins to be outputs:
  pinMode(rightLed, OUTPUT);
  pinMode(leftLed, OUTPUT);
}

void loop() {
  // Local variable to hold the pushbutton states
  int buttOneState;
  int buttTwoState;
  int buttThreeState;
  int buttFourState;

  // Reads the digital state of button with 
  // digitalRead() function and stores the
  // Value in the corresponding buttonState variable
  buttOneState = digitalRead(buttOne);
  buttTwoState = digitalRead(buttTwo);
  buttThreeState = digitalRead(buttThree);
  buttFourState = digitalRead(buttFour);

  // a pull-up resistor is used 
  // When the button is pressed, the power is grounded and moved to LOW state
  if (buttOneState == LOW) {//checks for LOW state
    serves++;
    setPointA++;
    gamePointIncrementer();
    serveSwap();
    refreshLCD();
    delay(300);
  }
  if (buttTwoState == LOW) {
    decrementChecker();
    setPointA--;
    refreshLCD();
    delay(300);
  }
  if (buttThreeState == LOW) {
    serves++;
    setPointB++;
    gamePointIncrementer();
    serveSwap();
    refreshLCD();
    delay(300);
  }
 if (buttFourState == LOW) {
    decrementChecker();
    setPointB--;
    refreshLCD();
    delay(300);
  }
}

// Checks to see if serves or scores should be reversed after decrementing;
void decrementChecker() {
  if (setPointA == 0 && setPointB == 0) {
    setPointA = tempA;
    setPointB = tempB;
    if (setPointA > setPointB) {
      gamePointA--;
    } else {
      gamePointB--;
    }
  }
}

// Checks to see if one or both players have a score over 10 points
// and that one score is at least 2 points ahead of the other.
// If true, then increment A or B game points and reset serve points and serves to 0.
void gamePointIncrementer() {
  if ((setPointA - setPointB > 1 || setPointB - setPointA > 1) && (setPointA > 10 || setPointB > 10)) {
    if (setPointA > setPointB) {
      gamePointA++;
      tempLast = A;
    } else {
      gamePointB++;
      tempLast = B;
    }
    tempA = setPointA;
    tempB = setPointB;
    setPointA = 0;
    setPointB = 0;
    serves = 0;
  }
}

// Checks to see who is serving.
// Number of serves per side is reduced from 2 to 1 if either player has 10 points.
// A light on either side of the board indicates which side is serving.
void serveSwap() {
  int serveLimit;
  if (setPointA > 10 || setPointB > 10) {
    serveLimit = 1;
  } else {
    serveLimit = 2;
  }
  
  // If the serve limit is reached, the player display and scores swap sides
  int tempPoints;
  if (serves == serveLimit) {
    tempPoints = gamePointA;
    gamePointA = gamePointB;
    gamePointB = tempPoints;
    String tempNames = A;
    A = B;
    B = tempNames;
    tempPoints = setPointA;
    setPointA = setPointB;
    setPointB = tempPoints;
    serves = 0;

    // Turns on a light to display which side is serving
    if (A == "B") {
      digitalWrite(leftLed, HIGH);
      digitalWrite(rightLed, LOW);
      
     
    } else {
      digitalWrite(leftLed, LOW);
      digitalWrite(rightLed, HIGH);
      
      
    }
  }
}

// Updates the LCD to display the correct score.
// Also calls winnerWinner to check for a winner.
void refreshLCD() {

  // Clears the screen, sets the cursor and prints text or a score 6 times
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(A + "  ");
  lcd.setCursor(4, 0);
  lcd.print(String(gamePointA)); // int variables need to be converted to String
  lcd.setCursor(7, 0);
  lcd.print(": " + B + "  ");
  lcd.setCursor(13, 0);
  lcd.print(String(gamePointB));
  lcd.setCursor(0, 1);
  lcd.print("SET " + String(setPointA));
  lcd.setCursor(7, 1);
  lcd.print(": SET " + String(setPointB));
  winnerWinner();
}

// Checks for a winner.
// If true, displays celebratory strobe and disco lights.
void winnerWinner() {

  // Best of 5 would mean first player to 3 game points.
  if (gamePointA > 2 || gamePointB > 2) {
    String winner;
    if (gamePointA > gamePointB) {
      winner = "Player 1";
      digitalWrite(rightLed, HIGH);
      digitalWrite(leftLed, LOW);
    } else {
      winner = "Player 2";
      digitalWrite(leftLed, HIGH);
      digitalWrite(rightLed, LOW);
    }

    // Displays the winner for 2 seconds.
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(winner);
    lcd.setCursor(0, 1);
    lcd.print("Wins the game!");
    delay(3000);

    // Then celebration for an absurdly long time!
    for (int win = 0; win < 10; win++) {
      digitalWrite(leftLed, LOW);
      digitalWrite(rightLed, HIGH);
      
     
      lcd.clear();
      delay(300);
      
      digitalWrite(leftLed, HIGH);
      digitalWrite(rightLed, LOW);
      
      lcd.setCursor(0, 0);
      lcd.print("WINNER WINNER");
      lcd.setCursor(0, 1);
      lcd.print(winner);
      delay(300);
      digitalWrite(leftLed, LOW);
      digitalWrite(rightLed, LOW);  

    }
    gamePointA = 0;
    gamePointB = 0;
    setPointA = 0;
    setPointB = 0;
  }
}
