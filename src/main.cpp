#include <Arduino.h>

#include "led.h"
#include "helpers.h"
#include "controller.h"

LED led = LED(LED_BUILTIN);
Controller controller;

void setup()
{
  Serial.begin(9600);
  connect_to_wifi();
}

void loop()
{
  led.blink_once(1000);
  led.blink_once(1000);

  controller.run();
}