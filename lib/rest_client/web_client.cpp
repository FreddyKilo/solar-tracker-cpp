#include <Arduino.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include "web_client.h"

WebClient::WebClient()
{
    m_world_time_api_url = "http://worldtimeapi.org";
}

DynamicJsonDocument WebClient::ntp_get_current_time()
{
    WiFiUDP ntp_udp;
    NTPClient ntp_client(ntp_udp, "pool.ntp.org");

    ntp_client.begin();
    ntp_client.update();
    DynamicJsonDocument current_time = _map_result_from_ntp_client(ntp_client);
    ntp_client.end();

    return current_time;
}

DynamicJsonDocument WebClient::_map_result_from_ntp_client(NTPClient ntp_client)
{
    DynamicJsonDocument result_map(1024);
    int formatted_hour = (ntp_client.getHours() + m_utc_offset_hours + 24) % 24;

    result_map["hours"] = formatted_hour;
    result_map["minites"] = ntp_client.getMinutes();
    result_map["seconds"] = ntp_client.getSeconds();

    return result_map;
}

DynamicJsonDocument WebClient::get_time_by_ip()
{
    DynamicJsonDocument json_response(1024);

    String path = "/api/ip";
    m_http_client.begin(m_wifi_client, m_world_time_api_url + path);
    int response_code = m_http_client.GET();

    if (response_code == 200)
    {
        String response = m_http_client.getString();
        json_response = _deserialize_response(json_response, response);
    }

    m_http_client.end();

    json_response["code"] = response_code;
    return json_response;
}

DynamicJsonDocument WebClient::_deserialize_response(DynamicJsonDocument doc, String response)
{
    deserializeJson(doc, response);
    return doc;
}