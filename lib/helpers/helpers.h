#ifndef HELPERS_H
#define HELPERS_H

#include <Arduino.h>
#include <string>

int time_to_seconds(std::string time);
int time_until_start(int current_time, int start_time);
std::string to_readable_time(std::string current_time);
int voltage_to_percentage(float voltage);
int map_azimuth_to_microsec(float sun_azimuth);
int map_altitude_to_microsec(float sun_altitude);
float map_float(float input, float in_min, float in_max, float out_min, float out_max);

#endif