#include "panel_servo.h"
#include "constants.h"
#include "helpers.h"

using namespace constants;

PanelServo::PanelServo(int pinout, int min_microseconds, int max_microseconds)
{
    _pinout = pinout;
    _min = min_microseconds;
    _max = max_microseconds;
}

void PanelServo::set_target(float angle, int speed)
{
    _servo.attach(_pinout, _min, _max);

    if (speed > MAX_SERVO_SPEED)
        speed = MAX_SERVO_SPEED;
    if (speed < 1)
        speed = 1;

    int target = 0;
    
    if (_pinout == D5)
    {
        target = map_azimuth_to_microsec(angle);
        if (_microsec == 0)
            _microsec = map_azimuth_to_microsec(DEFAULT_AZIMUTH_POSITION);
    }
    else if (_pinout == D6)
    {
        target = map_altitude_to_microsec(angle);
        if (_microsec == 0)
            _microsec = map_altitude_to_microsec(DEFAULT_ALTITUDE_POSITION);
    }

    while (_microsec != target)
    {
        int diff = target - _microsec;
        int increment = diff / abs(diff) * speed;

        if (abs(increment) > abs(diff))
            _microsec = target;
        else
            _microsec += increment;

        _servo.writeMicroseconds(_microsec);
    }

    _servo.detach();
}

int PanelServo::get_angle()
{
    return _servo.read();
}

int PanelServo::map_azimuth_to_microsec(float sun_azimuth)
{
    return map_float(sun_azimuth, 60, 300, _max, _min);
}

int PanelServo::map_altitude_to_microsec(float sun_altitude)
{
    return map_float(sun_altitude, 180, 0, _min, _max);
}