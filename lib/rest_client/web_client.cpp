#include "web_client.h"
#include "secrets.h"

using namespace std;

WebClient::WebClient()
{
    m_ip_geolocation_domain = "http://api.ipgeolocation.io";
}

DynamicJsonDocument WebClient::get_astronomy_data()
{
    String path = "/astronomy?apiKey=" + String(IP_GEOLOCATION_API_KEY);

    return _get(m_ip_geolocation_domain + path);
}

DynamicJsonDocument WebClient::_get(String url)
{
    DynamicJsonDocument json_response(1024);
    WiFiClient m_wifi_client;
    HTTPClient m_http_client;

    m_http_client.begin(m_wifi_client, url);
    int response_code = m_http_client.GET();

    if (response_code == HTTP_CODE_OK)
    {
        String response = m_http_client.getString();
        deserializeJson(json_response, response);
    }

    m_http_client.end();

    json_response["code"] = response_code;
    return json_response;
}
