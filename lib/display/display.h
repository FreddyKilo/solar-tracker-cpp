#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Display
{
public:
    Display(void);

    void init(void);
    void set_header(String line_1, String line_2);
    void display_status(String line_1, String line_2, String line_3);
    void display_current_positions(float azimuth, float altitude);
    void sleep(void);

private:
    Adafruit_SSD1306 _display;
    int get_cursor_start_centered(String text, int char_width);
};

#endif