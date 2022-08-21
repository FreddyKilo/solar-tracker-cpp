#include <ESP8266_ISR_Servo.h>
#include "panel_servo.h"

PanelServo::PanelServo(int pinout, int min_microseconds, int max_microseconds)
{
    _pinout = pinout;
    _servo_id = ISR_Servo.setupServo(pinout, min_microseconds, max_microseconds);
}

void PanelServo::set_target(int target, int speed)
{
    if (_pinout == D5) target = map_azimuth_angle(target);
    if (speed > 100) speed = 100;
    if (speed < 0) speed = 0;

    Serial.println("setting servo on pin " + String(_pinout) + " to " + String(target) + " degrees");

    while (_angle != target)
    {
        int diff = target - _angle;
        int increment = diff / abs(diff);

        ISR_Servo.setPosition(_servo_id, _angle + increment);

        _angle += increment;
        delay(200 - speed * 2);
    }
}

int PanelServo::get_angle()
{
    return ISR_Servo.getPosition(_servo_id);
}

int PanelServo::map_azimuth_angle(int sun_azimuth)
{
  return 270 - sun_azimuth;
}