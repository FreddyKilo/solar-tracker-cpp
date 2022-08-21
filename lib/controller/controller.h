#ifndef CONTROLLER
#define CONTROLLER

#include <ArduinoJson.h>

#include "web_client.h"
#include "panel_servo.h"

class Controller
{
public:
    Controller(void);

    void run(void);
    void set_default_position(void);
    void calibrate_servos(void);

private:
    WebClient _web_client;
    DynamicJsonDocument _astronomy_data;
    float _sun_azimuth;
    float _sun_altitude;
    PanelServo _azimuth_servo;
    PanelServo _altitude_servo;
    DynamicJsonDocument _log_message;
    int _tracking_delay;

    void get_astronomy_data(void);
};

#endif
