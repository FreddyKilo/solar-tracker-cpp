#include <ESP8266_ISR_Servo.h>
#include "panel_servo.h"

PanelServo::PanelServo(int pinout, int min_microseconds, int max_microseconds)
{
    _servo_id = ISR_Servo.setupServo(pinout, min_microseconds, max_microseconds);
}

void PanelServo::set_target(int angle)
{
    ISR_Servo.setPosition(_servo_id, angle);
}

int PanelServo::get_angle(){
    return ISR_Servo.getPosition(_servo_id);
}
