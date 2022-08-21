#include <Arduino.h>
#include "led.h"

LED::LED(int pin_number)
{
    m_pin_number = pin_number;
}

void LED::blink_once(int delay_seconds)
{
    int ms_on = 15;
    pinMode(m_pin_number, OUTPUT);

    digitalWrite(LED_BUILTIN, LOW);
    delay(ms_on);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delay_seconds - ms_on);
}

void LED::set_state(int state)
{
    m_state = state;
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, m_state);
}
