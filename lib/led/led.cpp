#include <Arduino.h>
#include "led.h"

LED::LED(int pin_number)
{
    m_pin_number = pin_number;
}

void LED::blink_once(int delay_seconds)
{
    pinMode(m_pin_number, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delay_seconds);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delay_seconds);
}

void LED::set_state(int state)
{
    m_state = state;
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, m_state);
}
