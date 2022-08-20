#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

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
  controller.run();
  led.blink_once(500);
}