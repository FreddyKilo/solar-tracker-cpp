#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
    constexpr double DEEP_SLEEP_SECOND = 1e6;
    
    constexpr int DELAY_SECOND = 1000;
    constexpr int DELAY_MINUTE = 60 * DELAY_SECOND;
    constexpr int SECONDS_IN_DAY = 86400;

    constexpr int DEFAULT_AZIMUTH_POSITION = 180;
    constexpr int DEFAULT_ALTITUDE_POSITION = 0;

    constexpr auto LOG_NAME_LIGHT_SLEEP = "tracker-v2-light-sleep";
    constexpr auto LOG_NAME_DEEP_SLEEP = "tracker-v2-deep-sleep";
}

#endif