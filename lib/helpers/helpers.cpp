#include "helpers.h"
#include "constants.h"

using namespace std;
using namespace constants;

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

std::string to_readable_time(string current_time)
{
  string hours = current_time.substr(0, 2);
  string minutes = current_time.substr(3, 2);

  return hours.append(":").append(minutes);
}

int voltage_to_percentage(float voltage)
{
  return map_float(voltage, MIN_BATTERY, MAX_BATTERY, 0, 100);
}

float map_float(float input, float in_min, float in_max, float out_min, float out_max)
{
  return (input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
