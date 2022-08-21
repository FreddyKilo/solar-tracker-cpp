#include <Arduino.h>
#include <unity.h>
#include <string>
#include "helpers.h"

void setUp(void) {}

void tearDown(void) {}

void test_time_to_seconds()
{
    std::string time = "01:00";
    int seconds = time_to_seconds(time);
    TEST_ASSERT_EQUAL(60, seconds);

    time = "01:30:54.345";
    seconds = time_to_seconds(time);
    TEST_ASSERT_EQUAL(90, seconds);
}

void test_seconds_to_sunrise()
{
    // test before midnight
    int current_time = 79200; // 10:00PM
    int sunrise_time = 21600; //  6:00AM
    int expected     = 28800; // 8 hours
    int to_sunrise = seconds_to_sunrise(current_time, sunrise_time);

    TEST_ASSERT_EQUAL(expected, to_sunrise);

    // test after midnight
    current_time =  3600; // 1:00AM
    sunrise_time = 21600; // 6:00AM
    expected     = 18000; // 5 hours
    to_sunrise = seconds_to_sunrise(current_time, sunrise_time);

    TEST_ASSERT_EQUAL(expected, to_sunrise);
}

void setup()
{
    UNITY_BEGIN();

    RUN_TEST(test_time_to_seconds);
    RUN_TEST(test_seconds_to_sunrise);
    
    UNITY_END();
}

void loop() {}
