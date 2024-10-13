#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>

// Initialize the LCD (address 0x27, 16 chars, 2 lines)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// LED pins: Green LED is on the 4th pin (index 3)
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8};  // 6 white LEDs + 1 green LED
const int numLEDs = 7;
int greenLEDIndex = 3;  // Green LED is at pin 4 (index 3 in array)

// Buzzer pin
const int buzzerPin = 11;  // Add the buzzer pin

// Button pins
const int startButtonPin = 9;
const int scoreButtonPin = 10;

// Variables
int score = 0;
int highScore = 0;
int currentLED = 0;  // Index of the currently lit LED
bool gameStarted = false;
unsigned long lastLEDUpdate = 0;  // For tracking LED timing
unsigned long ledInterval = 100;
bool scoreUpdated = false;

unsigned long gameStartTime = 0;  // Time when the game starts
unsigned long gameDuration = 30000;  // 30 seconds game time
bool gameEnded = false;
int remainingTime = 30;  // Timer countdown starts at 30 seconds

unsigned long lastTimerUpdate = 0;  // For tracking timer display updates

// Create Bounce objects for button debouncing
Bounce startButton = Bounce();
Bounce scoreButton = Bounce();

// Function declarations (before they are called)
void displayWelcomeScreen();
void resetGame();
void playBuzzer(bool play);

void setup() {
  // Set up LEDs
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  // Set up buzzer pin
  pinMode(buzzerPin, OUTPUT);

  // Set up buttons
  pinMode(startButtonPin, INPUT_PULLUP);
  pinMode(scoreButtonPin, INPUT_PULLUP);

  // Initialize Bounce objects for buttons
  startButton.attach(startButtonPin);
  startButton.interval(50);  // Increase debounce time to 50 ms
  scoreButton.attach(scoreButtonPin);
  scoreButton.interval(50);  // Increase debounce time to 50 ms

  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Display welcome message and high score
  displayWelcomeScreen();
}

void loop() {
  // Update the state of the buttons
  startButton.update();
  scoreButton.update();

  // Wait for the player to start the game
  if (!gameStarted && !gameEnded) {
    if (startButton.fell()) {  // Check for a falling edge on the start button
      gameStarted = true;
      gameStartTime = millis();
      remainingTime = 30;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Game Started!");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Score: ");
      lcd.print(score);
      lcd.setCursor(0, 1);
      lcd.print("Timer: ");
      lcd.print(remainingTime);
    }
    return;
  }

  unsigned long currentTime = millis();

  // End the game after 30 seconds
  if (gameStarted && (currentTime - gameStartTime >= gameDuration)) {
    endGame();
    return;
  }

  // Update the countdown timer every second
  if (gameStarted && (currentTime - lastTimerUpdate >= 1000)) {
    remainingTime--;
    lastTimerUpdate = currentTime;

    // Clear the previous timer and score
    lcd.setCursor(7, 0);
    lcd.print("   ");  // Clear old score (handles up to 3 digits)
    lcd.setCursor(7, 1);
    lcd.print("   ");  // Clear old timer (handles up to 3 digits)

    // Update the score and timer display
    lcd.setCursor(7, 0);
    lcd.print(score);
    lcd.setCursor(7, 1);
    lcd.print(remainingTime);
  }

  if (currentTime - lastLEDUpdate >= ledInterval && gameStarted) {
    // Turn off all LEDs before lighting the next one
    for (int i = 0; i < numLEDs; i++) {
        digitalWrite(ledPins[i], LOW);
    }

    // Move to the next LED in sequence
    currentLED++;
    if (currentLED >= numLEDs) {
        currentLED = 0;  // Reset to the first LED if we reach the end
    }

    // Turn on the current LED
    digitalWrite(ledPins[currentLED], HIGH);

    // Check if the current LED is the green LED
    if (currentLED == greenLEDIndex) {
      playBuzzer(true);  // Play sound if it's the green LED
    } else {
      playBuzzer(false);  // Stop sound for other LEDs
    }

    lastLEDUpdate = currentTime;
    scoreUpdated = false;
  }

  // Check for button press to score
  if (scoreButton.fell() && !scoreUpdated && gameStarted) {
    scoreUpdated = true;

    if (currentLED == greenLEDIndex) {
      score += 10;
      ledInterval = max(ledInterval - 10, 100);  // Speed up, but don't go below 100 ms
    } else {
      score = max(score - 5, 0);  // Prevent score going below 0
      ledInterval = min(ledInterval + 10, 400);  // Slow down, but don't go above 400 ms
    }

    delay(100);  // Short delay to prevent accidental double presses

    // Clear previous score before updating
    lcd.setCursor(7, 0);
    lcd.print("   ");  // Clear old score (up to 3 digits)

    // Update the score and timer on the LCD
    lcd.setCursor(7, 0);
    lcd.print(score);
    lcd.setCursor(7, 1);
    lcd.print(remainingTime);
  }
}

void endGame() {
  gameEnded = true;
  gameStarted = false;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Over!");
   noTone(buzzerPin);  // Add this line to stop any sound from the buzzer

  // Check if the player's score is a new high score
  if (score > highScore) {
    highScore = score;  // Update the high score
    lcd.setCursor(0, 1);
    lcd.print("New High Score!");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("High Score: ");
    lcd.print(highScore);
  }

  delay(5000);  // Delay to allow player to see the end screen
  
  // Reset variables properly to avoid affecting the next game
  resetGame();  // Call a function to reset all variables and start fresh
}

void resetGame() {
  // Reset all variables
  score = 0;
  ledInterval = 100;
  currentLED = 0;
  scoreUpdated = false;
  remainingTime = 30;  // Reset timer for the next game
  gameEnded = false;

  displayWelcomeScreen();  // Back to welcome screen
}

void displayWelcomeScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("Carnival Lights!");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Highscore: ");
  lcd.print(highScore);
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press Btn 1 to");
  lcd.setCursor(0, 1);
  lcd.print(" Start");

  gameEnded = false;
}

// Function to play or stop the buzzer
void playBuzzer(bool play) {
  if (play) {
    tone(buzzerPin, 1000);  // Play a 1000Hz tone
  } else {
    noTone(buzzerPin);  // Stop the tone
  }
}
