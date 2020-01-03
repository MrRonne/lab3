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

static bool p1KeyDownMatrix[rowAmount][colAmount];
static bool p2KeyDownMatrix[rowAmount][colAmount];

byte p1RowPins[rowAmount] = { 29, 27, 25, 23 };
byte p1ColPins[colAmount] = { 28, 26, 24, 22 };
byte p2RowPins[rowAmount] = { 37, 35, 33, 31 };
byte p2ColPins[colAmount] = { 36, 34, 32, 30 };

char message[] = { keyNone, keyNone };

void setup()
{
  for (int i = 0; i < rowAmount; i++) {
    pinMode(p1RowPins[i], OUTPUT);
    pinMode(p2RowPins[i], OUTPUT);
    digitalWrite(p1RowPins[i], HIGH);
    digitalWrite(p2RowPins[i], HIGH);
  }

  for (int i = 0; i < colAmount; i++) {
    pinMode(p1ColPins[i], INPUT);
    pinMode(p2ColPins[i], INPUT);
    digitalWrite(p1ColPins[i], HIGH);
    digitalWrite(p2ColPins[i], HIGH);
  }

  Serial.begin(115200);
  Wire.begin(address);
  Wire.onRequest(handleRequest);
}

void loop()
{
  char p1CurrentKey = getKey(p1RowPins, p1ColPins);
  char p2CurrentKey = getKey(p2RowPins, p2ColPins);
  message[0] = p1CurrentKey;
  message[1] = p2CurrentKey;
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
