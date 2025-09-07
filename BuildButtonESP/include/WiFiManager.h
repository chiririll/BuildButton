#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

enum class WiFiClientType
{
    Nobody = 0,
    ActionRunner = 1 << 0,
    Updater = 1 << 1,
};

class WiFiManager
{
public:
    void init();
    bool loop();

    void setClient(WiFiClientType client, bool active);

private:
    uint8_t m_clients;
};

#endif // WIFI_MANAGER_H