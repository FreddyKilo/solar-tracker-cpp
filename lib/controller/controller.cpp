#include <ArduinoJson.h>
#include <string>

#include "controller.h"
#include "web_client.h"
#include "panel_servo.h"
#include "helpers.h"
#include "constants.h"

using namespace std;
using namespace constants;

Controller::Controller()
    : _azimuth_servo(D5, 400, 2400),
      _altitude_servo(D6, 400, 2400)
{
    WebClient _web_client;
    _tracking_delay = 5 * DELAY_MINUTE;
}

void Controller::run()
{
    DynamicJsonDocument astronomy_data = _web_client.get_astronomy_data();
    _sun_azimuth = astronomy_data["sun_azimuth"];
    _sun_altitude = astronomy_data["sun_altitude"];

    if (_sun_altitude > 0)
    {
        _azimuth_servo.set_target(_sun_azimuth, 50);
        _altitude_servo.set_target(_sun_altitude, 50);

        delay(_tracking_delay);
    }
    else // after sunset
    {
        int current_time = time_to_seconds(astronomy_data["current_time"]);
        int sunrise = time_to_seconds(astronomy_data["sunrise"]);
        int delay_time = seconds_to_sunrise(current_time, sunrise);

        set_default_position();
        ESP.deepSleep(delay_time * DEEP_SLEEP_SECOND);
    }
}

void Controller::set_default_position()
{
    _azimuth_servo.set_target(DEFAULT_AZIMUTH_POSITION, 50);
    _altitude_servo.set_target(DEFAULT_ALTITUDE_POSITION, 50);
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