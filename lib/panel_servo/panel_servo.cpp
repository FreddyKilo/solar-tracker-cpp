#include "panel_servo.h"
#include "constants.h"
#include "helpers.h"

using namespace constants;

PanelServo::PanelServo(int pinout, int min_microseconds, int max_microseconds, int default_position)
{
    _pinout = pinout;
    _min = min_microseconds;
    _max = max_microseconds;
    _default_position = default_position;
}

void PanelServo::init()
{
    Serial.println("initializing servo to postition " + String(_default_position));

    _microsec = _default_position;
    _servo.attach(_pinout, _min, _max, _default_position);
}

void PanelServo::set_target(int target, int speed)
{
    if (target == _microsec)
    {
        _servo.detach();
        return;
    }

    _servo.attach(_pinout, _min, _max, _microsec);

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
