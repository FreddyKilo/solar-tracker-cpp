#ifndef WEB_CLIENT_H
#define WEB_CLIENT_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

class WebClient
{
public:
    WebClient(void);

    DynamicJsonDocument get_astronomy_data();
    DynamicJsonDocument log_info(String log_name, DynamicJsonDocument message_json);
    DynamicJsonDocument log_error(String log_name, DynamicJsonDocument message_json);

private:
    WiFiClient _wifi_client;
    HTTPClient _http_client;
    String _ip_geolocation_domain;
    String _dweet_domain;
    DynamicJsonDocument _json_response;

    DynamicJsonDocument _get(String url);
    DynamicJsonDocument _post(String url, String payload);
};

#endif