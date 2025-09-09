#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "StorageManager.h"

enum class WiFiClientType
{
    Nobody = 0,
    ActionRunner = 1 << 0,
    Updater = 1 << 1,
};

class WiFiManager
{
public:
    WiFiManager()
    {
        p_instance = this;
    }

    void init(StorageManager *storage);
    bool loop();

    void setClient(WiFiClientType client, bool active);
    void checkClients();

private:
    static WiFiManager *p_instance;

    StorageManager *m_storage;

    uint8_t m_clients;

    void connect();
    void disconnect();

    static void onScanCompleted(int n);
    void connectToKnownNetwork(int scannedCount);
};

#endif // WIFI_MANAGER_H