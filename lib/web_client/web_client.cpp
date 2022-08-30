#include "web_client.h"
#include "secrets.h"

using namespace std;

WebClient::WebClient() : _json_response(1024)
{
    _ip_geolocation_domain = "http://api.ipgeolocation.io";
    _dweet_domain = "http://dweet.io";
    _thingspeak_domain = "http://api.thingspeak.com";
    _aio_domain = "http://io.adafruit.com";
}

DynamicJsonDocument WebClient::get_astronomy_data()
{
    String path = "/astronomy?apiKey=" + String(IP_GEOLOCATION_API_KEY);
    return _get(_ip_geolocation_domain + path);
}

DynamicJsonDocument WebClient::log_info(String log_name, DynamicJsonDocument message_json)
{
    String path = "/dweet/for/";
    String url = _dweet_domain + path + log_name;

    String payload;
    serializeJson(message_json, payload);

    return _post(url, payload);
}

DynamicJsonDocument WebClient::plot_voltage_level_thingspeak(float voltage_level)
{
    String path = "/update?api_key=" + String(THINGSPEAK_API_WRITE_KEY) + "&field1=" + String(voltage_level);
    return _get(_thingspeak_domain + path);
}

DynamicJsonDocument WebClient::plot_voltage_level_aio(float voltage_level)
{
    String path = "/api/v2/" + String(AIO_USERNAME) + "/feeds/solar-tracker-feed/data?x-aio-key=" + String(AIO_KEY);
    String payload = "{\"value\":" + String(voltage_level) + "}";
    return _post(_aio_domain + path, payload);
}

DynamicJsonDocument WebClient::_get(String url)
{
    HTTPClient http_client;
    http_client.begin(_wifi_client, url);
    int response_code = http_client.GET();

    if (response_code == HTTP_CODE_OK)
    {
        String response = http_client.getString();
        deserializeJson(_json_response, response);
    }

    http_client.end();

    _json_response["code"] = response_code;
    return _json_response;
}

DynamicJsonDocument WebClient::_post(String url, String payload)
{
    HTTPClient http_client;
    http_client.begin(_wifi_client, url);
    http_client.addHeader("Content-Type", "application/json");
    int response_code = http_client.POST(payload);

    if (response_code == HTTP_CODE_OK)
    {
        String response = http_client.getString();
        deserializeJson(_json_response, response);
    }

    http_client.end();

    _json_response["code"] = response_code;
    return _json_response;
}