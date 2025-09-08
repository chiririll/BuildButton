#include "WiFiManager.h"

void WiFiManager::init(StorageManager *storage)
{
    m_storage = storage;
}

bool WiFiManager::loop()
{
    return false;
}

void WiFiManager::setClient(WiFiClientType client, bool active)
{
    auto clientByte = (uint8_t)client;
    m_clients = active
                    ? m_clients | clientByte
                    : m_clients & ~clientByte;

    checkClients();
}

void WiFiManager::checkClients()
{
    if (!m_clients && WiFi.status() == WL_CONNECTED)
    {
        disconnect();
    }
    else if (m_clients && WiFi.status() != WL_CONNECTED)
    {
        connect();
    }
}

void WiFiManager::connect()
{
    if (WiFi.status() == WL_CONNECTED)
        return;

    WiFi.mode(WIFI_STA);
    WiFi.begin();

    Serial.println("Scanning for WiFi networks...");

    auto n = WiFi.scanNetworks();

    if (!n)
    {
        Serial.println("No networks found.");
        WiFi.disconnect(true);
        return;
    }

    m_storage->begin();
    m_storage->wifiFile()->open();

    int bestRSSI = -999;
    String ssid;
    String password;

    for (int i = 0; i < n; i++)
    {
        String foundSSID = WiFi.SSID(i);
        int foundRSSI = WiFi.RSSI(i);

        if (foundRSSI <= bestRSSI)
            continue;

        auto foundPassword = m_storage->wifiFile()->getPassword(&foundSSID);
        if (foundPassword.isEmpty())
            continue;

        Serial.printf("Found known network: %s (RSSI %d)\n", foundSSID.c_str(), foundRSSI);

        ssid = foundSSID;
        password = foundPassword;
        bestRSSI = foundRSSI;
    }

    if (ssid.isEmpty())
    {
        Serial.println("No known networks available.");
        WiFi.disconnect(true);
        return;
    }

    Serial.printf("Connecting to best network: %s\n", ssid.c_str());
    WiFi.begin(ssid, password);

    m_storage->wifiFile()->close();
    m_storage->end();
}

void WiFiManager::disconnect()
{
    if (WiFi.status() != WL_CONNECTED)
        return;

    WiFi.disconnect(true);
}
