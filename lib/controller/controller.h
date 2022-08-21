#ifndef CONTROLLER
#define CONTROLLER

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
    float _sun_azimuth;
    float _sun_altitude;
    PanelServo _azimuth_servo;
    PanelServo _altitude_servo;
    int _tracking_delay;

    void get_astronomy_data(void);
};

#endif
