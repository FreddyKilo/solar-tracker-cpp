#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "helpers.h"
#include "secrets.h"
#include "constants.h"

using namespace std;
using namespace constants;

void connect_to_wifi()
{  
  Serial.print("\n\nAttempting connection");
  WiFi.mode(WIFI_STA);
  WiFi.begin(NETWORK_SSID, NETWORK_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  wifi_set_sleep_type(LIGHT_SLEEP_T);
  Serial.println("Connection established!");
}

int time_to_seconds(std::string time)
{
  int hours = stoi(time.substr(0, 2));
  int minutes = stoi(time.substr(3, 2));

  return (hours * 60 + minutes) * 60;
}

int seconds_to_sunrise(int current_time, int sunrise)
{
  if (current_time > sunrise)
  {
    int to_midnight = SECONDS_IN_DAY - current_time;
    return to_midnight + sunrise;
  }
  else
  {
    return sunrise - current_time;
  }
}
