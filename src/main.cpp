#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "led.h"
#include "web_client.h"
#include "secrets.h"

LED led = LED(LED_BUILTIN);
WebClient web_client;

void connect_to_wifi()
{
  WiFi.begin(NETWORK_SSID, NETWORK_PASS);
  Serial.print("\n\nAttempting connection to router");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}

void get_astronmy_data()
{
  DynamicJsonDocument response = web_client.get_astronomy_data();
  serializeJsonPretty(response, Serial);
}

void setup()
{
  Serial.begin(9600);
  connect_to_wifi();
  get_astronmy_data();
}

void loop()
{
  led.blink_once(500);
  delay(60000);
}