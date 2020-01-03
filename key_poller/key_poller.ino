#include <Wire.h>
#include <Servo.h>
#include <LedControl.h>
#include "Game.h"

#define INVALID_DIRECTION -1

const int keypadAdderss = 10;

const char keyUp = 'U';
const char keyDown = 'D';
const char keyRight = 'R';
const char keyLeft = 'L';
const char keyNone = 'N';

const int boardSizeX = 24;
const int boardSizeY = 8;

Game game(boardSizeX, boardSizeY);

LedControl ledDisplay = LedControl(26, 22, 24, 3);  // (DIN, CLK, CS, Количество дисплеев)

uint64_t lastGameUpdate;
uint64_t lastDisplayUpdate;
uint64_t lastFoodBlink;
const uint64_t gameUpdateDelayMs = 500;
const uint64_t displayUpdateDelayMs = 100;
const uint64_t foodBlinkDelayMs = 200;
bool showingFood = true;

char player1CurrentKey = keyNone;
char player2CurrentKey = keyNone;

const int player1ServoPin = 11;
const int player2ServoPin = 12;

const int positionIdle = 0;
const int positionWin = 90;

Servo player1Servo;
Servo player2Servo;

void setup() {
  for (int i=0; i<ledDisplay.getDeviceCount(); i++)
  {
    ledDisplay.shutdown(i, false);
    ledDisplay.setIntensity(i, 10);
    ledDisplay.clearDisplay(i);
  }
  /*
  Point point;
  point.x = 1;
  point.y = 2;
  drawPoint(point);
  */
  Serial.begin(115200);
  Wire.begin();
  
  player1Servo.attach(player1ServoPin);
  player2Servo.attach(player2ServoPin);
  player1Servo.write(positionIdle);
  player2Servo.write(positionIdle);
  delay(500);
  
  Serial.println(game.isRunning());
}

void loop() {
  if (game.isRunning()) {
    updateInput();
    updateGame();
    updateDisplay();
  }
  else {
    if (game.isDraw()) {
      player1Servo.write(positionWin);
      player2Servo.write(positionWin);
    }
    else if (game.isPlayer1Win()) {
      player1Servo.write(positionWin);
    }
    else {
      player1Servo.write(positionWin);
    }
    delay(500);
  }
}

Direction keyToDirection(char key) {
  switch(key) {
    case keyUp: return UP;
    case keyDown: return DOWN;
    case keyRight: return RIGHT;
    case keyLeft: return LEFT;
  }
  return INVALID_DIRECTION;
}

char readKey() {
  Wire.requestFrom(keypadAdderss, 1);
  if (Wire.available()) {
    return Wire.read();
  }
}

void drawPoint(Point point) {
  ledDisplay.setLed(0, boardSizeY - 1 - point.y, point.x, true);
}

void drawSnake(Snake &snake) {
  for (int i = 0; i < snake.getSize(); i++) {
    drawPoint(snake.getPosition(i));
  }
}

void drawFood(Point &food) {
  if (showingFood) {
    drawPoint(food);
  }
  if (millis() - lastFoodBlink > foodBlinkDelayMs) {
    showingFood = !showingFood;
    lastFoodBlink = millis();
  }
}

void updateInput() {
  Wire.requestFrom(keypadAdderss, 1);
  if (Wire.available()) {
    char p1Key = Wire.read();
    char p2Key = Wire.read();
    if (p1Key != keyNone) {
      player1CurrentKey = p1Key;
    }
    if (p2Key != keyNone) {
      player2CurrentKey = p2Key;
    }
  }
}

void updateGame() {
  if (millis() - lastGameUpdate > gameUpdateDelayMs) {
    Direction player1Direction = keyToDirection(player1CurrentKey);
    if (player1Direction > INVALID_DIRECTION) {
      game.setPlayer1Direction(player1Direction);
    }
    Direction player2Direction = keyToDirection(player2CurrentKey);
    if (player2Direction > INVALID_DIRECTION) {
      game.setPlayer2Direction(player2Direction);
    }
    game.update();
    player1CurrentKey = keyNone;
    player2CurrentKey = keyNone;
    lastGameUpdate = millis();
  }
}

void updateDisplay() {
  if (millis() - lastDisplayUpdate > displayUpdateDelayMs) {
    Snake player1 = game.getPlayer1();
    Snake player2 = game.getPlayer2();
    Point food = game.getFood();
    for (int i = 0; i < ledDisplay.getDeviceCount(); i++) {
      ledDisplay.clearDisplay(i);
    }
    drawSnake(player1);
    drawSnake(player2);
    drawFood(food);
    lastDisplayUpdate = millis();
  }
}
