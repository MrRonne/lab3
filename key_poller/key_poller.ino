#include <Wire.h>
#include "Game.h"
#include "LedControl.h"

#define INVALID_DIRECTION -1

const char keyUp = 'U';
const char keyDown = 'D';
const char keyRight = 'R';
const char keyLeft = 'L';
const char keyNone = 'N';

const int keypadAdderss = 10;

const int boardSizeX = 8;
const int boardSizeY = 8;

Game game(boardSizeX, boardSizeY);

LedControl ledDisplay = LedControl(26, 22, 24, 1);  // (DIN, CLK, CS, Количество дисплеев)

uint64_t lastGameUpdate;
uint64_t lastDisplayUpdate;
uint64_t lastFoodBlink;
const uint64_t gameUpdateDelayMs = 500;
const uint64_t displayUpdateDelayMs = 100;
const uint64_t foodBlinkDelayMs = 200;
bool showingFood = true;

char currentKey = keyNone;

void setup()
{
  ledDisplay.shutdown(0, false);
  ledDisplay.setIntensity(0, 10);
  ledDisplay.clearDisplay(0);
  Point point;
  point.x = 1;
  point.y = 2;
  drawPoint(point);
  Serial.begin(115200);
  Wire.begin();
  Serial.println(game.isRunning());
}

void loop()
{
  if (game.isRunning()) {
    updateInput();
    updateGame();
    updateDisplay();
  }
}

Direction keyToDirection(char key)
{
  switch(key) {
    case keyUp: return UP;
    case keyDown: return DOWN;
    case keyRight: return RIGHT;
    case keyLeft: return LEFT;
  }
  return INVALID_DIRECTION;
}

char readKey()
{
  Wire.requestFrom(keypadAdderss, 1);
  if (Wire.available()) {
    return Wire.read();
  }
}

void drawPoint(Point point)
{
  ledDisplay.setLed(0, boardSizeY - 1 - point.y, point.x, true);
}

void drawSnake(Snake &snake)
{
  for (int i = 0; i < snake.getSize(); i++) {
    drawPoint(snake.getPosition(i));
  }
}

void drawFood(Point &food)
{
  if (showingFood) {
    drawPoint(food);
  }
  if (millis() - lastFoodBlink > foodBlinkDelayMs) {
    showingFood = !showingFood;
    lastFoodBlink = millis();
  }
}

void updateInput() {
  char key = readKey();
  if (key != keyNone) {
    currentKey = key;
  }
}

void updateGame()
{
  if (millis() - lastGameUpdate > gameUpdateDelayMs) {
    if (currentKey != keyNone) {
      game.setSnakeDirection(keyToDirection(currentKey));
    }
    game.update();
    currentKey = keyNone;
    lastGameUpdate = millis();
  }
}

void updateDisplay()
{
  if (millis() - lastDisplayUpdate > displayUpdateDelayMs) {
    Snake snake = game.getSnake();
    Point food = game.getFood();
    ledDisplay.clearDisplay(0);
    drawSnake(snake);
    drawFood(food);
    lastDisplayUpdate = millis();
  }
}
