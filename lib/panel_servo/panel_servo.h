#ifndef PANEL_SERVO
#define PANEL_SERVO

class PanelServo
{
public:
    PanelServo(int pinout, int min_microseconds, int max_microseconds);

    void set_target(int angle, int speed);
    int get_angle();
    void set_speed(int speed);
    int map_azimuth_angle(int sun_azimuth);

private:
    int _pinout;
    int _servo_id;
    float _angle;
    int _speed;
};

#endif