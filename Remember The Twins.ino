// Partha Roy Ballave              Remember The Twins                Jan 25, 2022
// This program is paired with an arduino uno device, breadboard, LEDS and pushbuttons to create a challenging memory game.
// The objective of the game is to remember 4 different combinations of a pair of LEDS in the same order that they lit up.
// Everytime the user passes a level, the time between the combinations decreases making it harder for the user to remember. 

const int numRows = 2;
const int numCols = 2;
int ledPins[numRows][numCols] = {
  {6, 8},
  {11, 13}
};

// Define button pins
int buttonPins[numRows][numCols] = {
  {2, 3},
  {5, 12}
};

const int resetButton = 9;  

// Create an array to store the LED combinations
int ledCombinations[4][2];
int currentCombination = 0;
int level = 1;
int correctCombinations = 0;
int totalCombinations = 4;
bool lightsOver = false;
bool button1Pressed = false;
bool button2Pressed = false;

void setup() {
  // set pin modes for LEDs
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      pinMode(ledPins[i][j], OUTPUT);
      pinMode(buttonPins[i][j], INPUT);
    }
  }
pinMode(resetButton, INPUT);
randomSeed(analogRead(A0));
Serial.begin(9600);
}

void loop() {
 
  //program restart option
  if (digitalRead(resetButton) == HIGH) {
    level = 1;
    correctCombinations = 0;
    lightsOver = false;
    currentCombination = 0;
    Serial.println("Reset button pressed. Game reset.");
    delay(1000);
  }
  
  //ensuring that a different pair of led lights up
   if (!lightsOver) {
    int row1, col1, row2, col2;
    do {
      row1 = random(numRows);
      col1 = random(numCols);
      row2 = random(numRows);
      col2 = random(numCols);
    } while (row1 == row2 && col1 == col2);

    digitalWrite(ledPins[row1][col1], HIGH);
    digitalWrite(ledPins[row2][col2], HIGH);

    // store the current combination
    ledCombinations[currentCombination][0] = ledPins[row1][col1];
    ledCombinations[currentCombination][1] = ledPins[row2][col2];

    if (level == 1) {
      delay(2000);
    } else if (level == 2) {
      delay(1500);
    } else if (level == 3) {
      delay(1000);
    }

    // turn off the LEDs
    digitalWrite(ledPins[row1][col1], LOW);
    digitalWrite(ledPins[row2][col2], LOW);

    // increment the combination counter
    currentCombination++;
    if (currentCombination >= 4) {
      lightsOver = true;
      Serial.println("You may recreate the combinations now.");
    }
  } else {
    // check for button presses to recreate the combinations
    for (int i = 0; i < currentCombination; i++) {
      int button1 = -1;
      int button2 = -1;
      while (button1 == -1 || button2 == -1) {
        for (int row = 0; row < numRows; row++) {
          for (int col = 0; col < numCols; col++) {
            if (digitalRead(buttonPins[row][col]) == HIGH) {
              if (ledPins[row][col] == ledCombinations[i][0]) {
                button1 = buttonPins[row][col];
              }
              else if (ledPins[row][col] == ledCombinations[i][1]) {
                button2 = buttonPins[row][col];
              }
            }
          }
        }
      }
    
     
    //if correct combo, level up
   if (button1 != -1 && button2 != -1) {
        correctCombinations++;
        digitalWrite(ledCombinations[i][0], HIGH);
        digitalWrite(ledCombinations[i][1], HIGH);
        delay(1000);
        digitalWrite(ledCombinations[i][0], LOW);
        digitalWrite(ledCombinations[i][1], LOW);
        Serial.print("Correct combinations: ");
        Serial.println(correctCombinations);
        if (correctCombinations == totalCombinations) {
          level++;
          Serial.print("Level up! New level: ");
          Serial.println(level);
        }
      } else { // incorrect combo 
        if (correctCombinations != totalCombinations) {
          level = 1;
          correctCombinations = 0;
          lightsOver = false;
          currentCombination = 0;
          Serial.println("Incorrect, please try again.");
          Serial.println("Level reset to 1.");
        }
      }
      //max level, restart
       if (correctCombinations == totalCombinations) {
    level++;
    correctCombinations = 0;
    lightsOver = false;
    currentCombination = 0;
    if (level > 3) {
      level = 3;
      Serial.println("Maximum level reached. Well done!");
    } else {
      Serial.print("Level up! New level: ");
      Serial.println(level);
    }
      }
        }
          }

          
}







  
