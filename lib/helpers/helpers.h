#ifndef HELPERS
#define HELPERS

#include <Arduino.h>
#include <string>

int time_to_seconds(std::string time);
int seconds_to_sunrise(int current_time, int sunrise);
std::string to_readable_time(std::string current_time);

#endif