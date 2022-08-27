#include <Arduino.h>

#include "led.h"
#include "controller.h"
#include "constants.h"

using namespace constants;

LED led(LED_BUILTIN);
Controller controller;

void setup()
{
  Serial.begin(9600);
  controller.connect_to_wifi();
}

void loop()
{
  led.blink_many(3, 120);
  controller.run();
}