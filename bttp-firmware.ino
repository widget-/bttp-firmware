#include <Keyboard.h>
#include "config.h"

#define BUFFER_SIZE 64

int thresholds[NUM_STEPS] = {500, 500, 400, 500};
int readings[NUM_STEPS];

char buffer[BUFFER_SIZE];

void processSerial()
{
  while (Serial.available() > 0)
  {
    size_t bytes_read = Serial.readBytesUntil(
        '\n', buffer, BUFFER_SIZE - 1);
    buffer[bytes_read] = '\0';

    if (bytes_read == 0)
    {
      return;
    }

    switch (buffer[0])
    {
    case 'v':
    case 'V':
      printValues();
      break;
    case 't':
    case 'T':
      printThresholds();
      break;
    default:
      updateThresholds(bytes_read);
      break;
    }
  }
}

void printValues()
{
  for (int i = 0; i < NUM_STEPS; ++i)
  {
    Serial.print(" ");
    Serial.print(readings[i]);
  }
  Serial.print("\n");
}

void printThresholds()
{
  for (int i = 0; i < NUM_STEPS; ++i)
  {
    Serial.print(" ");
    Serial.print(thresholds[i]);
  }
  Serial.print("\n");
}

void updateThresholds(size_t bytes_read)
{
  if (bytes_read < 2 || bytes_read > 5)
  {
    return;
  }

  size_t step_index = buffer[0] - '0';
  if (step_index < 0 || step_index >= NUM_STEPS)
  {
    return;
  }

  thresholds[step_index] = atoi(buffer + 1);
  printThresholds();
}

void setup()
{
  Serial.begin(BAUD_RATE);
}

void loop()
{
  processSensors();
  processSerial();
}

void processSensors()
{
  for (int i = 0; i < NUM_STEPS; ++i)
  {
    readings[i] = analogRead(pins[i]);
    if (readings[i] > thresholds[i])
    {
      Keyboard.press(buttons[i]);
    }
    else
    {
      Keyboard.release(buttons[i]);
    }
  }
}
