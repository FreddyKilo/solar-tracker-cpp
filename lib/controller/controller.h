#ifndef CONTROLLER
#define CONTROLLER

#include "web_client.h"
#include "panel_servo.h"

class Controller
{
public:
    Controller(void);

    void run(void);

private:
    WebClient _web_client;
    float _sun_azimuth;
    float _sun_altitude;
    PanelServo _x_servo;
    PanelServo _y_servo;

    int _tracking_delay;

    void get_astronomy_data();
    void position_servos_default();
};

#endif
