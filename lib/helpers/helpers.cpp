#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "helpers.h"
#include "secrets.h"

using namespace std;

void connect_to_wifi()
{
  Serial.print("\n\nAttempting connection");
  WiFi.begin(NETWORK_SSID, NETWORK_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connection established!");
}

int time_to_seconds(std::string time)
{
  int hours = stoi(time.substr(0, 2));
  int minutes = stoi(time.substr(3, 2));

  return hours * minutes * 60;
}