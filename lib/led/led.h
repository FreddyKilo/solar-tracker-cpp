#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED
{
public:
    LED(int pin_number);

    void set_state(int state);
    void blink_once(int delay_seconds);
    void blink_many(int count, int delay_seconds);

private:
    int _pin_number;
    int _state;
    int _LED_LOW = HIGH;
    int _LED_HIGH = LOW;
};

#endif