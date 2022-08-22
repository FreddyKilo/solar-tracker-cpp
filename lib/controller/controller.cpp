#include <string>

#include "controller.h"
#include "helpers.h"
#include "constants.h"

using namespace std;
using namespace constants;

Controller::Controller()
    : _astronomy_data(1024),
      _azimuth_servo(D5, 400, 2400),
      _altitude_servo(D6, 400, 2400),
      _log_message(512),
      _display(128, 64, &Wire, -1)
{
    WebClient _web_client;
    _tracking_delay = 5 * DELAY_MINUTE;
}

void Controller::run()
{
    Serial.println("\n\n=====Tracking Controller running=====");

    _astronomy_data = _web_client.get_astronomy_data();
    _sun_azimuth = _astronomy_data["sun_azimuth"];
    _sun_altitude = _astronomy_data["sun_altitude"];

    _log_message["currentAzimuth"] = _sun_azimuth;
    _log_message["currentAltitude"] = _sun_altitude;
    _log_message["currentTime"] = _astronomy_data["current_time"];

    if (_sun_altitude > 0)
    {
        _azimuth_servo.set_target(_sun_azimuth, 50);
        _altitude_servo.set_target(_sun_altitude, 50);

        _web_client.log_info(LOG_NAME_LIGHT_SLEEP, _log_message);
        Serial.println("light sleeping for " + String(_tracking_delay / DELAY_MINUTE) + " minutes");
        display_positions();
        delay(_tracking_delay);
    }
    else // after sunset
    {
        int current_time = time_to_seconds(_astronomy_data["current_time"]);
        int sunrise = time_to_seconds(_astronomy_data["sunrise"]);
        int delay_time = seconds_to_sunrise(current_time, sunrise);

        set_default_position();

        _web_client.log_info(LOG_NAME_DEEP_SLEEP, _log_message);
        Serial.println("deep sleeping for " + String(delay_time / 60) + " minutes");
        display_positions();
        _display.dim(true);
        ESP.deepSleep(delay_time * DEEP_SLEEP_SECOND);
    }
}

void Controller::set_default_position()
{
    Serial.println("setting defaut position...");

    _azimuth_servo.set_target(DEFAULT_AZIMUTH_POSITION, 70);
    _altitude_servo.set_target(DEFAULT_ALTITUDE_POSITION, 70);
}

void Controller::calibrate_servos()
{
    _altitude_servo.set_target(0, 100);
    delay(1000);
    _altitude_servo.set_target(180, 100);
    delay(2000);

    _azimuth_servo.set_target(0, 100);
    delay(1000);
    _azimuth_servo.set_target(180, 100);
    delay(2000);
}

void Controller::init_display()
{
    _display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    _display.clearDisplay();
    _display.setTextColor(WHITE);
    
    display_header();
}

void Controller::display_header()
{
    _display.setTextSize(1);
    _display.setCursor(6, 0);
    _display.println("-- SOLAR TRACKER --");
    _display.setCursor(51, 8);
    _display.println("v2.0");
    _display.display();
}

void Controller::display_positions()
{
    _display.clearDisplay();
    display_header();

    _display.setCursor(3, 20);
    _display.println("  Current Position");
    _display.setTextSize(2);
    _display.setCursor(9, 32);
    _display.println("X: " + String(_sun_azimuth));
    _display.setCursor(9, 50);
    _display.println("Y: " + String(_sun_altitude));
    _display.display();
}