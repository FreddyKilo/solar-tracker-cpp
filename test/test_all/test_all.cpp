#include <Arduino.h>
#include <unity.h>
#include <string>
#include "helpers.h"
#include "panel_servo.h"

void setUp(void) {}

void tearDown(void) {}

void test_time_to_seconds()
{
    std::string time = "01:00";
    int seconds = time_to_seconds(time);
    TEST_ASSERT_EQUAL(3600, seconds);

    time = "01:30:54.345";
    seconds = time_to_seconds(time);
    TEST_ASSERT_EQUAL(5400, seconds);
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

void test_map_azimuth_angle()
{
    // the 0 degree postion of the servo used for azimuth is at the 3 o'clock position
    // and 180 is counter clockwise to 9 o'clock
    
    int sunrise_azimuth = 90; // due east
    int sunset_azimuth = 270; // due west

    PanelServo servo = PanelServo(0, 0, 0);

    int angle = servo.map_azimuth_angle(sunrise_azimuth);
    TEST_ASSERT_EQUAL(180, angle);

    angle = servo.map_azimuth_angle(sunset_azimuth);
    TEST_ASSERT_EQUAL(0, angle);
}

void setup()
{
    UNITY_BEGIN();

    RUN_TEST(test_time_to_seconds);
    RUN_TEST(test_seconds_to_sunrise);
    RUN_TEST(test_map_azimuth_angle);
    
    UNITY_END();
}

void loop() {}
