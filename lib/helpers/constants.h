#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
    constexpr double DEEP_SLEEP_SECOND = 1e6;
    constexpr double ADJUSTED_DEEP_SLEEP_SECOND = 1.07e6; // the Wemos RTC is slightly off, need to add compensation
    constexpr int DELAY_SECOND = 1000;
    constexpr int DELAY_MINUTE = 60 * DELAY_SECOND;
    constexpr int TRACKING_DELAY = 10 * DELAY_MINUTE;
    constexpr int SECONDS_IN_DAY = 86400;
    constexpr int MAX_DEEP_SLEEP_SECONDS = 10800; // any more than this (3 hours) risks the device never waking, this is a hardware limitation that needs a workaround

    // servo calibration params
    constexpr int AZIMUTH_SERVO_MIN_MICROSEC = 555;
    constexpr int AZIMUTH_SERVO_MAX_MICROSEC = 2250;
    constexpr int ALTITUDE_SERVO_MIN_MICROSEC = 425;
    constexpr int ALTITUDE_SERVO_MAX_MICROSEC = 2370;
    constexpr int DEFAULT_AZIMUTH_POSITION = 1402;
    constexpr int DEFAULT_ALTITUDE_POSITION = 1397;
    constexpr int MAX_SERVO_SPEED = 10;

    constexpr auto LOG_NAME_RUNNING = "tracker-v2-running";
    constexpr auto LOG_NAME_DEEP_SLEEP = "tracker-v2-deep-sleep";

    // oled display params
    constexpr int DISPLAY_PIXEL_WIDTH = 128;
    constexpr int DISPLAY_PIXEL_HEIGHT = 64;
    constexpr auto DEFAULT_HEADER_LINE_1 = "-- SOLAR TRACKER --";
    constexpr auto DEFAULT_HEADER_LINE_2 = "v2.0";
    constexpr auto STATUS_HEADER = "-- STATUS --";

    // battery monitoring params
    constexpr int MIN_ADC_INPUT = 461;
    constexpr int MAX_ADC_INPUT = 600;
    constexpr float MIN_BATTERY = 3.2;
    constexpr float MAX_BATTERY = 4.2;

    constexpr std::uint8_t RUN_MODE_CALIBRATE_SERVOS = 0b00000001;
    constexpr std::uint8_t RUN_MODE_CALIBRATE_VOLTAGE = 0b00000010;
}

#endif