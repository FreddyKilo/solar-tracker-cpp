#include "display.h"
#include "constants.h"

#define DISPLAY_PIXEL_WIDTH 128
#define DISPLAY_PIXEL_HEIGHT 64
#define CHAR_SIZE_1_WIDTH 6
#define CHAR_SIZE_2_WIDTH 12

using namespace constants;

Display::Display()
    : _display(128, 64, &Wire, -1)
{
}

void Display::init()
{
    _display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    _display.clearDisplay();
    _display.setTextColor(WHITE);
}

void Display::set_header(String line_1, String line_2)
{
    int cursor_x_1 = get_cursor_start_centered(line_1, CHAR_SIZE_1_WIDTH);
    int cursor_x_2 = get_cursor_start_centered(line_2, CHAR_SIZE_1_WIDTH);

    _display.setTextSize(1);
    _display.setCursor(cursor_x_1, 0);
    _display.println(line_1);
    _display.setCursor(cursor_x_2, 8);
    _display.println(line_2);
    _display.display();
}

void Display::display_status(String line_1, String line_2, String line_3)
{
    _display.clearDisplay();
    set_header("", STATUS_HEADER);

    int cursor_x_1 = get_cursor_start_centered(line_1, CHAR_SIZE_1_WIDTH);
    int cursor_x_2 = get_cursor_start_centered(line_2, CHAR_SIZE_1_WIDTH);
    int cursor_x_3 = get_cursor_start_centered(line_3, CHAR_SIZE_1_WIDTH);

    _display.setTextSize(1);
    _display.setCursor(cursor_x_1, 20);
    _display.println(line_1);
    _display.setCursor(cursor_x_2, 32);
    _display.println(line_2);
    _display.setCursor(cursor_x_3, 44);
    _display.println(line_3);
    _display.display();
    delay(DELAY_SECOND);
}

void Display::display_current_positions(String subtitle, float azimuth, float altitude)
{
    String azimuth_key = "X: ";
    String altitude_key = "Y: ";

    if (azimuth >= 100.0) azimuth_key = "X:";
    if (altitude < 10.0) altitude_key = "Y:  ";

    String azimuth_position = azimuth_key + String(azimuth);
    String altitude_position = altitude_key + String(altitude);

    int subtitle_x = get_cursor_start_centered(subtitle, CHAR_SIZE_1_WIDTH);
    int azimuth_x = get_cursor_start_centered(azimuth_position, CHAR_SIZE_2_WIDTH);
    int altitude_x = get_cursor_start_centered(altitude_position, CHAR_SIZE_2_WIDTH);

    _display.clearDisplay();
    set_header(DEFAULT_HEADER_LINE_1, DEFAULT_HEADER_LINE_2);

    _display.setCursor(subtitle_x, 20);
    _display.println(subtitle);

    _display.setTextSize(2);
    _display.setCursor(azimuth_x, 32);
    _display.println(azimuth_position);
    _display.setCursor(altitude_x, 50);
    _display.println(altitude_position);
    _display.display();
}

void Display::sleep()
{
    _display.clearDisplay();
    _display.display();
}

int Display::get_cursor_start_centered(String text, int char_width)
{
    return (DISPLAY_PIXEL_WIDTH - text.length() * char_width) / 2;
}