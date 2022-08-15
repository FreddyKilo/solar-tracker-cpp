#include "web_client.h"

using namespace std;

WebClient::WebClient()
{
    m_world_time_api_url = "http://worldtimeapi.org";
    m_utc_offset = make_tuple(0, 0);
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
        string utc_offset = json_response["utc_offset"];
        _set_utc_offset(utc_offset);
    }

    m_http_client.end();

    json_response["code"] = response_code;
    return json_response;
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
    tuple<int, int> utc_offset = _get_utc_offset();
    int adjusted_hour = (ntp_client.getHours() + get<0>(utc_offset) + 24) % 24;
    int adjusted_minutes = (ntp_client.getMinutes() + get<1>(utc_offset) + 60) % 60;

    result_map["hours"] = adjusted_hour;
    result_map["minites"] = adjusted_minutes;
    result_map["seconds"] = ntp_client.getSeconds();

    return result_map;
}

DynamicJsonDocument WebClient::_deserialize_response(DynamicJsonDocument doc, String response)
{
    deserializeJson(doc, response);
    return doc;
}

tuple<int, int> WebClient::_get_utc_offset()
{
    return m_utc_offset;
}

void WebClient::_set_utc_offset(string utc_offset)
{
    string hours = utc_offset.substr(0, utc_offset.find(":"));
    string minutes = utc_offset.substr(utc_offset.find(":") + 1);

    m_utc_offset = make_tuple(stoi(hours), stoi(minutes));
}