#ifndef PANEL_SERVO
#define PANEL_SERVO

#include <Servo.h>

class PanelServo
{
public:
    PanelServo(int pinout, int min_microseconds, int max_microseconds);

    void set_target(float angle, int speed);
    int get_angle();
    int map_azimuth_to_microsec(float sun_azimuth);
    int map_altitude_to_microsec(float sun_altitude);

private:
    Servo _servo;
    int _pinout;
    int _min;
    int _max;
    float _microsec;
    int _speed;
};

#endif