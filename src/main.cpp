#include <Arduino.h>

#include "led.h"
#include "controller.h"

LED led(LED_BUILTIN);
Controller controller;

void setup()
{
  Serial.begin(9600);
  controller.connect_to_wifi();
}

void loop()
{
  led.blink_once(3000);
  controller.run();
}