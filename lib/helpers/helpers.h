#ifndef HELPERS
#define HELPERS

#include <string>

void connect_to_wifi();
int time_to_seconds(std::string time);
int seconds_to_sunrise(int current_time, int sunrise);

#endif