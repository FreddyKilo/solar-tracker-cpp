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
