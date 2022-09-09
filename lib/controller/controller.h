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
    void run(std::uint8_t mode = 0b00000000);
    void test_charging_shutoff(void);

private:
    WebClient _web_client;
    DynamicJsonDocument _astronomy_data;
    float _sun_azimuth;
    float _sun_altitude;
    float _prev_azimuth;
    float _prev_altitude;
    PanelServo _azimuth_servo;
    PanelServo _altitude_servo;
    DynamicJsonDocument _log_message;
    Display _display;

    float read_voltage_level(void);
    void get_astronomy_data(void);
    void calibrate_servos(void);
    void calibrate_votage_reading(void);
    void deep_sleep(int sleep_seconds);
    void wait_for_wifi(void);
};

#endif
