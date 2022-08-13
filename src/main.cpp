#include <Arduino.h>
#include "led.h"

LED led = LED(LED_BUILTIN);
int delay_seconds;

void setup() {
  Serial.begin(9600);
  delay_seconds = 500;
}

void loop() {
  Serial.write("blink!\n");
  led.blink_once(delay_seconds);
}