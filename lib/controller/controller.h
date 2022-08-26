#ifndef CONTROLLER
#define CONTROLLER

#include <ArduinoJson.h>

#include "web_client.h"
#include "panel_servo.h"
#include "display.h"

class Controller
{
public:
    Controller(void);

    void connect_to_wifi(void);
    void run(void);
    void calibrate_votage_reading(void);
    void calibrate_servos(void);

private:
    WebClient _web_client;
    DynamicJsonDocument _astronomy_data;
    float _sun_azimuth;
    float _sun_altitude;
    PanelServo _azimuth_servo;
    PanelServo _altitude_servo;
    DynamicJsonDocument _log_message;
    Display _display;

    float read_voltage_level(void);
    void get_astronomy_data(void);
    void deep_sleep(int sleep_seconds);
};

#endif
