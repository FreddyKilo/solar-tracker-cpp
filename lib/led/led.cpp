#include "led.h"

LED::LED(int pin_number)
{
    _pin_number = pin_number;
    pinMode(LED_BUILTIN, OUTPUT);
    set_state(_LED_LOW);
}

void LED::blink_once(int delay_seconds)
{
    int ms_on = 15;
    pinMode(_pin_number, OUTPUT);

    set_state(_LED_HIGH);
    delay(ms_on);
    set_state(_LED_LOW);
    delay(delay_seconds - ms_on);
}

void LED::blink_many(int count, int delay_seconds)
{
    while (count > 0)
    {
        blink_once(delay_seconds);
        count--;
    }
}

void LED::set_state(int state)
{
    _state = state;
    digitalWrite(LED_BUILTIN, _state);
}
