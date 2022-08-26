#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
    constexpr double DEEP_SLEEP_SECOND = 1e6;
    constexpr int DELAY_SECOND = 1000;
    constexpr int DELAY_MINUTE = 60 * DELAY_SECOND;
    constexpr int TRACKING_DELAY = 5 * DELAY_MINUTE;
    constexpr int SECONDS_IN_DAY = 86400;

    constexpr int AZIMUTH_SERVO_MIN_MICROSEC = 420;
    constexpr int AZIMUTH_SERVO_MAX_MICROSEC = 2400;
    constexpr int ALTITUDE_SERVO_MIN_MICROSEC = 430;
    constexpr int ALTITUDE_SERVO_MAX_MICROSEC = 2425;
    constexpr int DEFAULT_AZIMUTH_POSITION = 180;
    constexpr int DEFAULT_ALTITUDE_POSITION = 0;
    constexpr int MAX_SERVO_SPEED = 10;

    constexpr auto LOG_NAME_LIGHT_SLEEP = "tracker-v2-light-sleep";
    constexpr auto LOG_NAME_DEEP_SLEEP = "tracker-v2-deep-sleep";

    constexpr int DISPLAY_PIXEL_WIDTH = 128;
    constexpr int DISPLAY_PIXEL_HEIGHT = 64;
    constexpr auto DEFAULT_HEADER_LINE_1 = "-- SOLAR TRACKER --";
    constexpr auto DEFAULT_HEADER_LINE_2 = "v2.0";
    constexpr auto STATUS_HEADER = "-- STATUS --";

    // ranges for mapping ADC input to voltage
    constexpr int MIN_ADC_INPUT = 460;
    constexpr int MAX_ADC_INPUT = 600;
    constexpr float MIN_BATTERY = 3.2;
    constexpr float MAX_BATTERY = 4.2;
}

#endif