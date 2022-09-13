#ifndef PANEL_SERVO_H
#define PANEL_SERVO_H

#include <Servo.h>

class PanelServo
{
public:
    PanelServo(int pinout, int min_microseconds, int max_microseconds, int default_position);

    void init(void);
    void set_target(int target, int speed);
    int get_angle(void);

private:
    Servo _servo;
    int _pinout;
    int _min;
    int _max;
    int _default_position;
    float _microsec;
    int _speed;
};

#endif