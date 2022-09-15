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
      _azimuth_servo(D5, AZIMUTH_SERVO_MIN_MICROSEC, AZIMUTH_SERVO_MAX_MICROSEC, DEFAULT_AZIMUTH_POSITION),
      _altitude_servo(D6, ALTITUDE_SERVO_MIN_MICROSEC, ALTITUDE_SERVO_MAX_MICROSEC, DEFAULT_ALTITUDE_POSITION),
      _log_message(512)
{
    WebClient _web_client;
}

void Controller::init()
{
    pinMode(D3, OUTPUT);

    _azimuth_servo.init();
    _altitude_servo.init();
    delay(1000);

    _display.init();
}

void Controller::connect_to_wifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(NETWORK_SSID, NETWORK_PASS);
    wait_for_wifi();

    wifi_set_sleep_type(MODEM_SLEEP_T);
    _display.display_status("", "Connected!", "");
}

void Controller::run(std::uint8_t mode)
{
    if (mode == RUN_MODE_CALIBRATE_SERVOS)
        calibrate_servos();
    if (mode == RUN_MODE_CALIBRATE_VOLTAGE)
        calibrate_votage_reading();

    float voltage_level = read_voltage_level();
    int battery_percentage = voltage_to_percentage(voltage_level);

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
        Serial.println("previous_azimuth" + String(_prev_azimuth));
        if (_prev_azimuth == 0)
            _prev_azimuth = _sun_azimuth;
        if (_prev_altitude == 0)
            _prev_altitude = _sun_altitude;

        float leading_azimuth_target = 2 * _sun_azimuth - _prev_azimuth;
        float leading_altitude_target = 2 * _sun_altitude - _prev_altitude;
        _prev_azimuth = _sun_azimuth;
        _prev_altitude = _sun_altitude;

        _azimuth_servo.set_target(map_azimuth_to_microsec(leading_azimuth_target), 5);
        _altitude_servo.set_target(map_altitude_to_microsec(leading_altitude_target), 5);

        _web_client.log_data_aio(voltage_level, battery_percentage, leading_azimuth_target, leading_altitude_target);

        _display.display_current_positions("Current Position", leading_azimuth_target, leading_altitude_target);
        delay(TRACKING_DELAY);
    }
    else // after sunset
    {
        _web_client.log_data_aio(voltage_level, battery_percentage, 180, 90);

        int sleep_seconds = seconds_to_sunrise(current_time, sunrise);
        deep_sleep(sleep_seconds);
    }
}

// Gets an input value from the ADC pin A0 then maps it to a reabable voltage level
float Controller::read_voltage_level()
{
    _display.display_status("Reading voltage level", "", "");

    // need to turn off charging circuit and wifi modem to get a stable and accurate reading from A0
    digitalWrite(D3, HIGH);
    WiFi.forceSleepBegin(0);
    delay(1000);

    int sum = 0;
    int reading_count = 8;
    for (int i = 0; i < reading_count; i++)
    {
        sum += analogRead(A0);
        delay(50);
    }

    int reading = sum / reading_count;

    digitalWrite(D3, LOW);
    WiFi.forceSleepWake();
    wait_for_wifi();

    // calibrating the voltage measurement with a multimeter resulted in 460 - 600 mapping for 3.20v - 4.20v
    // need a custom mapper to return a float
    return map_float(reading, MIN_ADC_INPUT, MAX_ADC_INPUT, MIN_BATTERY, MAX_BATTERY);
}

void Controller::deep_sleep(int sleep_seconds)
{
    _display.display_status("Until next", "sunrise...", "Goodnight (-.-)");
    delay(1000);

    _azimuth_servo.set_target(DEFAULT_AZIMUTH_POSITION, MAX_SERVO_SPEED);
    _altitude_servo.set_target(DEFAULT_ALTITUDE_POSITION, MAX_SERVO_SPEED);

    if (sleep_seconds > MAX_DEEP_SLEEP_SECONDS)
        sleep_seconds = MAX_DEEP_SLEEP_SECONDS;
    else
        sleep_seconds += 300; // give a 5 min buffer due to astronomy API inaccuracies

    uint64_t sleep_time = sleep_seconds * ADJUSTED_DEEP_SLEEP_SECOND;
    _log_message["sleepMinutes"] = sleep_seconds / 60;
    _web_client.log_info(LOG_NAME_DEEP_SLEEP, _log_message);

    _display.sleep();
    ESP.deepSleep(sleep_time);
}

void Controller::wait_for_wifi()
{
    _display.display_status("Waiting for WiFi", "to connect", "");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");
}

void Controller::calibrate_votage_reading()
{
    float voltage;
    while (true)
    {
        voltage = read_voltage_level();
        _display.display_status("voltage level", String(voltage), "");
    }
}

void Controller::calibrate_servos()
{
    String subtitle = "Calibrating Servos";
    float x_angle;
    float y_angle;

    while (true)
    {
        y_angle = 0; // to horizon
        _display.display_current_positions(subtitle, x_angle, y_angle);
        _altitude_servo.set_target(map_altitude_to_microsec(y_angle), 5);

        x_angle = 90; // east
        _display.display_current_positions(subtitle, x_angle, y_angle);
        _azimuth_servo.set_target(map_azimuth_to_microsec(x_angle), 5);

        y_angle = 90; // up
        _display.display_current_positions(subtitle, x_angle, y_angle);
        _altitude_servo.set_target(map_altitude_to_microsec(y_angle), 5);

        x_angle = 270; // west
        _display.display_current_positions(subtitle, x_angle, y_angle);
        _azimuth_servo.set_target(map_azimuth_to_microsec(x_angle), 5);
    }
}

void Controller::test_charging_shutoff()
{
    pinMode(D3, OUTPUT);

    while (true)
    {
        Serial.println("turning charge off");
        digitalWrite(D3, HIGH);
        delay(4000);
        Serial.println("turning charge on");
        digitalWrite(D3, LOW);
        delay(4000);
    }
}