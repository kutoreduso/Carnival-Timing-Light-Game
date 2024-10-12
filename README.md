# Carnival Lights Game

This is a simple **Carnival Lights Game** built using an Arduino, LEDs, buttons, an LCD display, and a passive buzzer. The objective of the game is to press the **Score Button** when the **Green LED** lights up. The game becomes progressively harder by increasing the speed of the LEDs.

## Features

- **LED Light Sequence**: 7 LEDs light up in sequence. The green LED triggers a sound using a passive buzzer.
- **LCD Display**: Displays the current score and remaining time.
- **Score Button**: Press the score button when the green LED lights up to earn points. Pressing on the wrong LED decreases your score.
- **Start Button**: Starts the game.
- **Timer**: Game duration is 30 seconds, displayed on the LCD.
- **High Score**: The game saves and displays the highest score achieved.

## Components

1. **Arduino** (e.g., Uno, Mega)
2. **7 LEDs** (6 White LEDs and 1 Green LED)
3. **Passive Buzzer**
4. **16x2 LCD Display** (I2C)
5. **2 Push Buttons** (Start and Score)
6. **Wires and Resistors**
7. **Breadboard**

## Circuit Diagram

1. **LEDs**: Connect the 7 LEDs to digital pins (2-8), with resistors in series.
2. **Buttons**: 
   - Start Button to pin 9.
   - Score Button to pin 10.
3. **Passive Buzzer**: Connect the buzzer to pin 11.
4. **LCD**: Connect the I2C pins (SDA, SCL) to the appropriate pins on your Arduino (A4, A5 on Uno).

## How It Works

1. **Game Start**: Press the **Start Button** to begin the game. The LEDs will start lighting up in sequence.
2. **Scoring**: When the **Green LED** lights up, press the **Score Button** to earn points.
   - If the Green LED is hit: +10 points, and the game speeds up.
   - If you miss and press on other LEDs: -5 points, and the game slows down.
3. **Timer**: The game lasts for 30 seconds, with the time displayed on the LCD.
4. **End of Game**: When time runs out, the LCD displays the final score. If you achieved a new high score, it is saved and displayed.

## Code

You can find the full code in the `CarnivalLightsGame.ino` file. Here's a brief overview:

```cpp
// LED pins: Green LED is on the 4th pin (index 3)
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8};

// Buzzer pin
const int buzzerPin = 11;

// Buttons
const int startButtonPin = 9;
const int scoreButtonPin = 10;

// Other game variables (score, timing, etc.)
...
