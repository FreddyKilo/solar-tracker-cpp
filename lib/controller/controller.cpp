#include <ArduinoJson.h>
#include <string>

#include "controller.h"
#include "web_client.h"
#include "panel_servo.h"
#include "helpers.h"

using namespace std;

Controller::Controller()
    : _x_servo(D5, 900, 2100),
      _y_servo(D6, 440, 2400)
{
    WebClient _web_client;
    _tracking_delay = 5 * 60 * 1000;
}

void Controller::run()
{
    DynamicJsonDocument astronomy_data = _web_client.get_astronomy_data();
    _sun_azimuth = astronomy_data["sun_azimuth"];
    _sun_altitude = astronomy_data["sun_altitude"];

    if (_sun_altitude > 0)
    {
        _x_servo.set_target(_sun_azimuth);
        _y_servo.set_target(_sun_altitude);
        delay(_tracking_delay);
    }
    else
    {
        int current_time = time_to_seconds(astronomy_data["current_time"]);
        int sunrise = time_to_seconds(astronomy_data["sunrise"]);

        int diff = 
        delay()
    }
}
