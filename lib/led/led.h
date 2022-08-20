#ifndef LED_H
#define LED_H

class LED
{
public:
    LED(int pin_number);

    void set_state(int state);
    void blink_once(int delay_seconds);

private:
    int m_pin_number;
    int m_state;
};

#endif