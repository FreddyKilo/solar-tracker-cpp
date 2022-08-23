#ifndef CONTROLLER
#define CONTROLLER

#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "web_client.h"
#include "panel_servo.h"
#include "display.h"

class Controller
{
public:
    Controller(void);

    void connect_to_wifi(void);
    void run(void);
    void set_default_position(void);
    void calibrate_servos(void);
    // void init_display(void);

private:
    WebClient _web_client;
    DynamicJsonDocument _astronomy_data;
    float _sun_azimuth;
    float _sun_altitude;
    PanelServo _azimuth_servo;
    PanelServo _altitude_servo;
    DynamicJsonDocument _log_message;
    int _tracking_delay;
    // Adafruit_SSD1306 _display;
    Display _display;

    void get_astronomy_data(void);
    // void display_header(void);
    // void display_status(String status);
    // void display_positions(void);
};

#endif
