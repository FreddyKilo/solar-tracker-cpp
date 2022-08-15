#ifndef Web_CLIENT_H
#define Web_CLIENT_H

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

class WebClient
{
private:
    WiFiClient m_wifi_client;
    HTTPClient m_http_client;
    int m_utc_offset_hours;
    String m_world_time_api_url;
    DynamicJsonDocument _map_result_from_ntp_client(NTPClient ntp_client);
    DynamicJsonDocument _deserialize_response(DynamicJsonDocument doc, String response);

public:
    WebClient(void);

    DynamicJsonDocument ntp_get_current_time(void);
    DynamicJsonDocument get_time_by_ip(void);
};

#endif