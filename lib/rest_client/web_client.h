#ifndef Web_CLIENT_H
#define Web_CLIENT_H

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

class WebClient
{
private:
    int m_utc_offset_hours;
    DynamicJsonDocument _map_result_from_time_client(NTPClient time_client);

public:
    WebClient(void);

    DynamicJsonDocument get_current_time(void);
};

#endif