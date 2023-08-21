#include "web_client.h"
#include "secrets.h"

using namespace std;

WebClient::WebClient() : _json_response(1024)
{
    _ip_geolocation_domain = "https://api.ipgeolocation.io";
    _dweet_domain = "https://dweet.io";
    _thingspeak_domain = "http://api.thingspeak.com";
    _aio_domain = "https://io.adafruit.com";
}

DynamicJsonDocument WebClient::get_astronomy_data()
{
    String path = "/astronomy?apiKey=" + String(IP_GEOLOCATION_API_KEY) + "&location=" + String(LOCATION);
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

void WebClient::log_data_aio(float voltage_level, int percentage, float azimuth, float altitude)
{
    String voltage_level_feed_name = "battery-voltage-level";
    String percentage_feed_name = "battery-percentage";
    String azimuth_feed_name = "azimuth-angle";
    String altitude_feed_name = "altitude-angle";

    String path = "/api/v2/" + String(AIO_USERNAME) + "/feeds/" + voltage_level_feed_name + "/data?x-aio-key=" + String(AIO_KEY);
    String payload = "{\"value\":" + String(voltage_level) + "}";
    _post(_aio_domain + path, payload);

    path = "/api/v2/" + String(AIO_USERNAME) + "/feeds/" + percentage_feed_name + "/data?x-aio-key=" + String(AIO_KEY);
    payload = "{\"value\":" + String(percentage) + "}";
    _post(_aio_domain + path, payload);

    path = "/api/v2/" + String(AIO_USERNAME) + "/feeds/" + azimuth_feed_name + "/data?x-aio-key=" + String(AIO_KEY);
    payload = "{\"value\":" + String(azimuth) + "}";
    _post(_aio_domain + path, payload);

    path = "/api/v2/" + String(AIO_USERNAME) + "/feeds/" + altitude_feed_name + "/data?x-aio-key=" + String(AIO_KEY);
    payload = "{\"value\":" + String(altitude) + "}";
    _post(_aio_domain + path, payload);
}

DynamicJsonDocument WebClient::_get(String url)
{
    std::unique_ptr<BearSSL::WiFiClientSecure> wifi_client(new BearSSL::WiFiClientSecure);
    wifi_client->setInsecure();

    _http_client.begin(*wifi_client, url);
    int response_code = _http_client.GET();

    String response = _http_client.getString();
    Serial.println("response from " + url + ": " + response);

    if (response_code == HTTP_CODE_OK)
    {
        deserializeJson(_json_response, response);
    }

    _http_client.end();

    _json_response["code"] = response_code;
    return _json_response;
}

DynamicJsonDocument WebClient::_post(String url, String payload)
{
    std::unique_ptr<BearSSL::WiFiClientSecure> wifi_client(new BearSSL::WiFiClientSecure);
    wifi_client->setInsecure();

    _http_client.begin(*wifi_client, url);
    _http_client.addHeader("Content-Type", "application/json");
    int response_code = _http_client.POST(payload);

    String response = _http_client.getString();
    Serial.println("response from " + url + ": " + response);

    if (response_code == HTTP_CODE_OK)
    {
        deserializeJson(_json_response, response);
    }

    _http_client.end();

    _json_response["code"] = response_code;
    return _json_response;
}