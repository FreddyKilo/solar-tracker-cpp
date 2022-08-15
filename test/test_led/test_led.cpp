#include <Arduino.h>
#include <unity.h>
#include "led.h"

void setUp(void) {}

void tearDown(void) {}

void test_led_builtin_pin_number(void)
{
    TEST_ASSERT_EQUAL(2, LED_BUILTIN);
}

void test_set_state(void)
{
    LED led = LED(LED_BUILTIN);

    led.set_state(LOW);
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_BUILTIN));
    
    led.set_state(HIGH);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_BUILTIN));
}

void setup()
{
    UNITY_BEGIN();

    RUN_TEST(test_led_builtin_pin_number);
    RUN_TEST(test_set_state);
    
    UNITY_END();
}

void loop() {}
