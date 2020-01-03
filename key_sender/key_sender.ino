#include <Wire.h>

const int address = 10;

const char keyUp = 'U';
const char keyDown = 'D';
const char keyRight = 'R';
const char keyLeft = 'L';
const char keyNone = 'N';

const byte rowAmount = 4;
const byte colAmount = 4;

char keyMatrix[rowAmount][colAmount] = {
  {keyNone, keyUp,    keyNone,  keyNone},
  {keyLeft, keyNone,  keyRight, keyNone},
  {keyNone, keyDown,  keyNone,  keyNone},
  {keyNone, keyNone,  keyNone,  keyNone}
};

static bool player1KeyDownMatrix[rowAmount][colAmount];
static bool player2KeyDownMatrix[rowAmount][colAmount];

byte player1RowPins[rowAmount] = { 29, 27, 25, 23 };
byte player1ColPins[colAmount] = { 28, 26, 24, 22 };
byte player2RowPins[rowAmount] = { 37, 35, 33, 31 };
byte player2ColPins[colAmount] = { 36, 34, 32, 30 };

char message[] = { keyNone, keyNone };

void setup()
{
  for (int i = 0; i < rowAmount; i++) {
    pinMode(player1RowPins[i], OUTPUT);
    pinMode(player2RowPins[i], OUTPUT);
    digitalWrite(player1RowPins[i], HIGH);
    digitalWrite(player2RowPins[i], HIGH);
  }

  for (int i = 0; i < colAmount; i++) {
    pinMode(player1ColPins[i], INPUT);
    pinMode(player2ColPins[i], INPUT);
    digitalWrite(player1ColPins[i], HIGH);
    digitalWrite(player2ColPins[i], HIGH);
  }

  Serial.begin(115200);
  Wire.begin(address);
  Wire.onRequest(handleRequest);
}

void loop()
{
  char player1CurrentKey = getKey(player1RowPins, player1ColPins);
  char player2CurrentKey = getKey(player2RowPins, player2ColPins);
  message[0] = player1CurrentKey;
  message[1] = player2CurrentKey;
  Serial.println(message);
}

char getKey(byte rowPins[], byte colPins[])
{
  char result = keyNone;
  for (int i = 0; i < rowAmount; i++) {
    for (int j = 0; j < colAmount; j++) {
      if (isKeyDown(rowPins, colPins, i, j)) {
        return keyMatrix[i][j];
      }
    } 
  }
  return keyNone;
}

bool isKeyDown(byte rowPins[], byte colPins[], int i, int j)
{
  bool result = false;
  digitalWrite(rowPins[i], LOW);
  if (digitalRead(colPins[j]) == LOW) {
    result = true;
  }
  digitalWrite(rowPins[i], HIGH);
  return result;
}

void writeMessage(char *s) 
{
  uint8_t c;
  while (c = *s++) {
    Wire.write(c);
  }
}

void handleRequest() {
  writeMessage(message);
}
