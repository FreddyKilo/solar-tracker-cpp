#include "panel_servo.h"

PanelServo::PanelServo(int pinout, int min_microseconds, int max_microseconds)
{
    _servo.attach(pinout, min_microseconds, max_microseconds);
    _pinout = pinout;
}

void PanelServo::set_target(int target)
{
    if (_pinout == D5) target = map_azimuth_angle(target);

    Serial.println("setting servo on pin " + String(_pinout) + " to " + String(target) + " degrees");

    while (_angle != target)
    {
        int diff = target - _angle;
        int increment = diff / abs(diff);

        _servo.write(_angle + increment);

        _angle += increment;
    }
}

int PanelServo::get_angle()
{
    return _servo.read();
}

int PanelServo::map_azimuth_angle(int sun_azimuth)
{
    if (sun_azimuth > 270) return 0;
    return 270 - sun_azimuth;
}