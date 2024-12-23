#include <LiquidCrystal.h>

// Pin definitions
const int RS_PIN = 8, EN_PIN = 9, D4_PIN = 4, D5_PIN = 5, D6_PIN = 6, D7_PIN = 7;
const int BACKLIGHT_PIN = 10;
const int BUTTON_INPUT_PIN = A0;

// Button definitions
const int BUTTON_DELAY = 200;

// Game names
const char* games[] = { "Reaction Time", "Memory Game", "Countdown", "Maze Game", "Guess Number" };
const int NUM_GAMES = sizeof(games) / sizeof(games[0]);

// LCD object
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

// Button voltage thresholds
struct Button {
  int voltageLimit;
  const char* name;
};

const Button buttons[] = {
  { 50, "Right" },
  { 150, "Up" },
  { 300, "Down" },
  { 500, "Left" },
  { 750, "Select" },
  { 1000, "Reset" },
  { 1024, "" }
};

const int BUTTON_RIGHT = 0;
const int BUTTON_UP = 1;
const int BUTTON_DOWN = 2;
const int BUTTON_LEFT = 3;
const int BUTTON_SELECT = 4;
const int BUTTON_RESET = 5;
const int NO_BUTTON = 6;

// Helper function to get button index
int getButtonIndex(int analogValue) {
  for (int i = 0; i < NO_BUTTON; i++) {
    if (analogValue < buttons[i].voltageLimit) {
      return i;
    }
  }
  return NO_BUTTON;
}

// Menu system for selecting games
int selectGame() {
  int currentSelection = 0;

  while (true) {
    // Display the current game selection
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Select Game:");
    lcd.setCursor(0, 1);
    lcd.print(games[currentSelection]);

    // Wait for button press
    int buttonIndex = NO_BUTTON;
    while (buttonIndex == NO_BUTTON) {
      buttonIndex = getButtonIndex(analogRead(BUTTON_INPUT_PIN));
      delay(BUTTON_DELAY);
    }

    // Handle button input
    if (buttonIndex == BUTTON_UP) {
      currentSelection = (currentSelection - 1 + NUM_GAMES) % NUM_GAMES;
    } else if (buttonIndex == BUTTON_DOWN) {
      currentSelection = (currentSelection + 1) % NUM_GAMES;
    } else if (buttonIndex == BUTTON_SELECT) {
      return currentSelection;
    }
  }
}

// Game 1: Reaction Time
void reactionTimeGame() {
  lcd.clear();
  lcd.print("Get Ready...");
  delay(random(2000, 5000));

  lcd.clear();
  lcd.print("Press: Select");

  unsigned long startTime = millis();
  while (true) {
    if (getButtonIndex(analogRead(BUTTON_INPUT_PIN)) == BUTTON_SELECT) {  // Select button
      unsigned long reactionTime = millis() - startTime;
      lcd.clear();
      lcd.print("Time: ");
      lcd.print(reactionTime);
      lcd.print(" ms");
      delay(3000);
      break;
    }
  }
}

// Game 2: Memory Game
void memoryGame() {
  lcd.clear();
  lcd.print("Memorize...");
  delay(1000);

  const char* sequence[] = { "Up", "Down", "Left", "Right", "Select" };
  int sequenceLength = random(3, 8);
  int randomSequence[sequenceLength];

  // Generate random sequence
  for (int i = 0; i < sequenceLength; i++) {
    lcd.clear();
    randomSequence[i] = random(0, 5);
    lcd.print(i);
    lcd.print(" - ");
    lcd.print(sequence[randomSequence[i]]);
    delay(1000);
  }

  lcd.clear();
  lcd.print("Repeat!");

  for (int i = 0; i < sequenceLength; i++) {
    int buttonIndex = NO_BUTTON;
    while (buttonIndex == NO_BUTTON) {
      buttonIndex = getButtonIndex(analogRead(BUTTON_INPUT_PIN));
      delay(BUTTON_DELAY);
    }
    if (strcmp(buttons[buttonIndex].name, sequence[randomSequence[i]]) != 0) {
      lcd.clear();
      lcd.print("Wrong!");
      delay(2000);
      return;
    }
    else
    {
      lcd.setCursor(0, 1);
      lcd.print(i + 1);
    }
    delay(BUTTON_DELAY);
  }

  lcd.clear();
  lcd.print("You win!");
  delay(2000);
}

// Game 3: Countdown Timer Challenge
void countdownTimerGame() {
  lcd.clear();
  int timeLeft = 5;

  while (timeLeft > 0) {
    lcd.setCursor(0, 0);
    lcd.print("Time: ");
    lcd.print(timeLeft);

    lcd.setCursor(0, 1);
    lcd.print("Press: Select");

    unsigned long startTime = millis();
    while (millis() - startTime < 1000) {
      if (getButtonIndex(analogRead(BUTTON_INPUT_PIN)) == BUTTON_SELECT) {
        lcd.clear();
        lcd.print("You win!");
        delay(2000);
        return;
      }
    }
    timeLeft--;
  }

  lcd.clear();
  lcd.print("Time's up!");
  delay(2000);
}

// Game 4: Maze Game (Simple Text-Based Navigation)
void mazeGame() {
  char maze[2][16] = {
    { 'O', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X' },
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
  };
  int playerX = 0, playerY = 0;

  while (true) {
    lcd.clear();
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 16; j++) {
        lcd.setCursor(j, i);
        lcd.print(maze[i][j]);
      }
    }

    int buttonIndex = NO_BUTTON;
    while (buttonIndex == NO_BUTTON) {
      buttonIndex = getButtonIndex(analogRead(BUTTON_INPUT_PIN));
      delay(BUTTON_DELAY);
    }

    maze[playerY][playerX] = ' ';
    if (buttonIndex == BUTTON_UP && playerY > 0) playerY--;
    if (buttonIndex == BUTTON_DOWN && playerY < 1) playerY++;
    if (buttonIndex == BUTTON_LEFT && playerX > 0) playerX--;
    if (buttonIndex == BUTTON_RIGHT && playerX < 15) playerX++;

    maze[playerY][playerX] = 'O';
    if (playerX == 15 && playerY == 0) {
      lcd.clear();
      lcd.print("You win!");
      delay(2000);
      return;
    }
  }
}

// Game 5: Guess the Number
void guessNumberGame() {
  lcd.clear();
  lcd.print("Guess Number");
  int target = random(1, 100);
  int guess = 50;

  while (true) {
    lcd.setCursor(0, 1);
    lcd.print("Your Guess: ");
    lcd.print(guess);

    int buttonIndex = NO_BUTTON;
    while (buttonIndex == NO_BUTTON) {
      buttonIndex = getButtonIndex(analogRead(BUTTON_INPUT_PIN));
      delay(BUTTON_DELAY);
    }

    if (buttonIndex == BUTTON_UP) guess++;
    if (buttonIndex == BUTTON_DOWN) guess--;
    if (buttonIndex == BUTTON_SELECT) {
      if (guess == target) {
        lcd.clear();
        lcd.print("Correct!");
        delay(2000);
        return;
      } else if (guess > target) {
        lcd.clear();
        lcd.print("Lower!");
        delay(1000);
      } else {
        lcd.clear();
        lcd.print("Higher!");
        delay(1000);
      }
    }
  }
}

// Main setup and loop
void setup() {
  pinMode(BACKLIGHT_PIN, OUTPUT);
  digitalWrite(BACKLIGHT_PIN, HIGH);
  lcd.begin(16, 2);
  lcd.print("Game Menu");
  delay(2000);
}

void loop() {
  int selectedGame = selectGame();

  while (true) {
    switch (selectedGame) {
      case 0: reactionTimeGame(); break;
      case 1: memoryGame(); break;
      case 2: countdownTimerGame(); break;
      case 3: mazeGame(); break;
      case 4: guessNumberGame(); break;
    }
  }
}
