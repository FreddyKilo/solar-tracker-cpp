#include <string>
#include <vector>
#include <ESP8266WiFi.h>

#include "controller.h"
#include "helpers.h"
#include "constants.h"
#include "secrets.h"

using namespace std;
using namespace constants;

Controller::Controller()
    : _astronomy_data(1024),
      _azimuth_servo(D5, AZIMUTH_SERVO_MIN_MICROSEC, AZIMUTH_SERVO_MAX_MICROSEC),
      _altitude_servo(D6, ALTITUDE_SERVO_MIN_MICROSEC, ALTITUDE_SERVO_MAX_MICROSEC),
      _log_message(512)
{
    WebClient _web_client;
    _display.init();
}

void Controller::connect_to_wifi()
{
    _display.display_status("", "Connecting to WiFi", "");

    WiFi.mode(WIFI_STA);
    WiFi.begin(NETWORK_SSID, NETWORK_PASS);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }

    wifi_set_sleep_type(LIGHT_SLEEP_T);
    _display.display_status("", "Connected!", "");
}

void Controller::run(std::uint8_t mode)
{
    if (mode == RUN_MODE_CALIBRATE_SERVOS)
        calibrate_servos();
    if (mode == RUN_MODE_CALIBRATE_VOLTAGE)
        calibrate_votage_reading();

    Serial.println("\n\n=====Tracking Controller running=====");

    float voltage_level = read_voltage_level();
    _display.display_status("Voltage level", String(voltage_level) + "v", "");

    _display.display_status("Getting astronomy", "data", "");
    _astronomy_data = _web_client.get_astronomy_data();
    _sun_azimuth = _astronomy_data["sun_azimuth"];
    _sun_altitude = _astronomy_data["sun_altitude"];

    _log_message["created"] = to_readable_time(_astronomy_data["current_time"]);
    _log_message["currentAzimuth"] = _sun_azimuth;
    _log_message["currentAltitude"] = _sun_altitude;
    _log_message["batteryVoltage"] = voltage_level;

    int current_time = time_to_seconds(_astronomy_data["current_time"]);
    int sunrise = time_to_seconds(_astronomy_data["sunrise"]);
    int sunset = time_to_seconds(_astronomy_data["sunset"]);

    if (sunrise <= current_time and current_time <= sunset)
    {
        _display.display_status("Setting azimuth", "servo", "");
        _azimuth_servo.set_target(_sun_azimuth, 5);
        _display.display_status("Setting altitude", "servo", "");
        _altitude_servo.set_target(_sun_altitude, 5);

        _web_client.log_info(LOG_NAME_LIGHT_SLEEP, _log_message);
        _display.display_current_positions("Current Position", _sun_azimuth, _sun_altitude);
        delay(TRACKING_DELAY);
    }
    else // after sunset
    {
        int sleep_seconds = seconds_to_sunrise(current_time, sunrise);
        deep_sleep(sleep_seconds);
    }
}

// Gets an average input value from the ADC pin A0 then maps it to a reabable voltage level
// Analog input of 0v - 3.3v maps to a digital range of 0 - 1023
// Reading will be of range 496 - 652 for a voltage range of 1.6v to 2.1 (using a voltage divider)
// Need to map input range of 496 - 652 to output 3.20 to 4.20 (low and high range of battery)
// The Arduino map library returns truncated integer value so a custom mapper was needed
float Controller::read_voltage_level()
{
    _display.display_status("Reading voltage level", "", "");

    int total = 0;
    int reading;
    int reading_count = 16;
    vector<int> adc_readings;
    adc_readings.reserve(reading_count);

    delay(500); // stabilize before reading
    for (int i = reading_count; i > 0; i--)
    {
        reading = analogRead(A0);
        total += reading;
        adc_readings.push_back(reading);
        delay(100);
    }

    // eliminate high and low reading for better accuracy with average
    int high = 0;
    int low = 1023;
    for (auto &reading : adc_readings)
    {
        if (reading > high)
            high = reading;
        if (reading < low)
            low = reading;
    }

    // average the total minus the high and low readings
    total -= high;
    total -= low;
    int average = total / (reading_count - 2);

    // calibrating the voltage measurement with a multimeter resulted in 460 - 600 mapping to 3.2v - 4.2v
    // need a custom mapper to return a float
    return (average - MIN_ADC_INPUT) * (MAX_BATTERY - MIN_BATTERY) / (MAX_ADC_INPUT - MIN_ADC_INPUT) + MIN_BATTERY;
}

void Controller::deep_sleep(int sleep_seconds)
{
    _display.display_status("Setting default", "position", "Goodnight (-.-) Zzz..");
    _azimuth_servo.set_target(DEFAULT_AZIMUTH_POSITION, MAX_SERVO_SPEED);
    _altitude_servo.set_target(DEFAULT_ALTITUDE_POSITION, MAX_SERVO_SPEED);

    if (sleep_seconds > MAX_DEEP_SLEEP_SECONDS)
        sleep_seconds = MAX_DEEP_SLEEP_SECONDS;
    else
        sleep_seconds += 300; // give a 5 min buffer due to astronomy API inaccuracies

    uint64_t sleep_time = sleep_seconds * ADJUSTED_DEEP_SLEEP_SECOND;
    _log_message["sleepMinutes"] = sleep_seconds / 60;
    _log_message["sleepSeconds"] = sleep_seconds;
    _log_message["adjustedSleepSeconds"] = sleep_time / DEEP_SLEEP_SECOND;
    _log_message["deepSleepMax"] = ESP.deepSleepMax();
    _web_client.log_info(LOG_NAME_DEEP_SLEEP, _log_message);

    _display.sleep();
    ESP.deepSleep(sleep_time);
}

void Controller::calibrate_votage_reading()
{
    float voltage;
    while (true)
    {
        voltage = read_voltage_level();
        _display.display_status("voltage level", String(voltage), "");
        delay(1000);
    }
}

void Controller::calibrate_servos()
{
    String subtitle = "Calibrating Servos";
    float x_angle;
    float y_angle;

    while (true)
    {
        y_angle = 90;
        _display.display_current_positions(subtitle, x_angle, y_angle);
        _altitude_servo.set_target(y_angle, 5); // to horizon

        x_angle = 93.80928;
        _display.display_current_positions(subtitle, x_angle, y_angle);
        _azimuth_servo.set_target(x_angle, MAX_SERVO_SPEED); // east

        y_angle = 0;
        _display.display_current_positions(subtitle, x_angle, y_angle);
        _altitude_servo.set_target(y_angle, MAX_SERVO_SPEED); // up

        x_angle = 255.93002;
        _display.display_current_positions(subtitle, x_angle, y_angle);
        _azimuth_servo.set_target(x_angle, MAX_SERVO_SPEED); // west
    }
}
