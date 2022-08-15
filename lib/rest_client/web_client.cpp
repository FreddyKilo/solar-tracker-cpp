#include <Arduino.h>
#include <NTPClient.h>
#include <ArduinoJson.h>

#include "web_client.h"

WebClient::WebClient()
{
    m_utc_offset_hours = -7;
}

DynamicJsonDocument WebClient::get_current_time()
{
    WiFiUDP m_ntp_udp;
    NTPClient m_time_client(m_ntp_udp, "pool.ntp.org");

    m_time_client.begin();
    m_time_client.update();

    DynamicJsonDocument current_time = _map_result_from_time_client(m_time_client);
    m_time_client.end();

    return current_time;
}

DynamicJsonDocument WebClient::_map_result_from_time_client(NTPClient time_client)
{
    DynamicJsonDocument result_map(1024);
    int formatted_hour = (time_client.getHours() + m_utc_offset_hours + 24) % 24;

    result_map["hours"] = formatted_hour;
    result_map["minites"] = time_client.getMinutes();
    result_map["seconds"] = time_client.getSeconds();

    return result_map;
}