#include <string>
#include <ESP8266WiFi.h>

#include "controller.h"
#include "helpers.h"
#include "constants.h"
#include "secrets.h"

using namespace std;
using namespace constants;

Controller::Controller()
    : _astronomy_data(1024),
      _azimuth_servo(D5, 400, 2400),
      _altitude_servo(D6, 400, 2400),
      _log_message(512)
//   _display(128, 64, &Wire, -1)
{
    WebClient _web_client;
    _tracking_delay = 5 * DELAY_MINUTE;
    _display.init();
}

void Controller::connect_to_wifi()
{
    Serial.print("\n\nAttempting connection");
    _display.display_status("", "Connecting to WiFi", "");

    WiFi.mode(WIFI_STA);
    WiFi.begin(NETWORK_SSID, NETWORK_PASS);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }

    wifi_set_sleep_type(LIGHT_SLEEP_T);
    Serial.println("Connection established!");
    _display.display_status("", "Connected!", "");
}

void Controller::run()
{
    Serial.println("\n\n=====Tracking Controller running=====");

    _display.display_status("Getting astronomy", "data", "");
    _astronomy_data = _web_client.get_astronomy_data();
    _sun_azimuth = _astronomy_data["sun_azimuth"];
    _sun_altitude = _astronomy_data["sun_altitude"];

    _log_message["currentAzimuth"] = _sun_azimuth;
    _log_message["currentAltitude"] = _sun_altitude;
    _log_message["currentTime"] = _astronomy_data["current_time"];

    if (_sun_altitude > 0)
    {
        _display.display_status("Setting azimuth", "servo", "");
        _azimuth_servo.set_target(_sun_azimuth, 80);
        _display.display_status("Setting altitude", "servo", "");
        _altitude_servo.set_target(_sun_altitude, 80);

        _web_client.log_info(LOG_NAME_LIGHT_SLEEP, _log_message);
        Serial.println("light sleeping for " + String(_tracking_delay / DELAY_MINUTE) + " minutes");
        _display.display_current_positions(_sun_azimuth, _sun_altitude);
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
        _display.display_status("", "GOODNIGHT :)", "");
        _display.sleep();
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

// void Controller::init_display()
// {
//     _display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
//     _display.clearDisplay();
//     _display.setTextColor(WHITE);

//     display_header();
// }

// void Controller::display_header()
// {
//     _display.setTextSize(1);
//     _display.setCursor(6, 0);
//     _display.println("-- SOLAR TRACKER --");
//     _display.setCursor(51, 8);
//     _display.println("v2.0");
//     _display.display();
// }

// void Controller::display_status(String status)
// {
//     _display.clearDisplay();
//     display_header();

//     _display.setTextSize(1);

//     _display.setCursor(0, 20);
//     _display.println("       Status:");

//     _display.setCursor(0, 30);
//     _display.println(status);

//     _display.display();
// }

// void Controller::display_positions()
// {
//     String azimuth_key = "X:  ";
//     String altitude_key = "Y:  ";

//     if (_sun_azimuth >= 100.0)
//         azimuth_key = "X: ";

//     if (_sun_altitude < 0.0)
//         altitude_key = "Y: ";

//     _display.clearDisplay();
//     display_header();

//     _display.setCursor(3, 20);
//     _display.println("  Current Position");
//     _display.setTextSize(2);
//     _display.setCursor(9, 32);
//     _display.println(azimuth_key + String(_sun_azimuth));
//     _display.setCursor(9, 50);
//     _display.println(altitude_key + String(_sun_altitude));
//     _display.display();
// }