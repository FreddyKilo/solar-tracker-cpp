#ifndef WEB_CLIENT_H
#define WEB_CLIENT_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

class WebClient
{
private:
    String m_ip_geolocation_domain;

    DynamicJsonDocument _get(String url);

public:
    WebClient(void);

    DynamicJsonDocument get_astronomy_data();
};

#endif