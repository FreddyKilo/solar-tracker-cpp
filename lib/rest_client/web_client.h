#ifndef Web_CLIENT_H
#define Web_CLIENT_H

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <tuple>
#include <string>

class WebClient
{
private:
    WiFiClient m_wifi_client;
    HTTPClient m_http_client;
    String m_world_time_api_url;
    std::tuple<int, int> m_utc_offset;

    DynamicJsonDocument _map_result_from_ntp_client(NTPClient ntp_client);
    DynamicJsonDocument _deserialize_response(DynamicJsonDocument doc, String response);
    std::tuple<int, int> _get_utc_offset(void);
    void _set_utc_offset(std::string utc_offset);

public:
    WebClient(void);

    DynamicJsonDocument ntp_get_current_time(void);
    DynamicJsonDocument get_time_by_ip(void);
};

#endif