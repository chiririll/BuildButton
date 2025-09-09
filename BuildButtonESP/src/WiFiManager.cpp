#include "WiFiManager.h"

WiFiManager *WiFiManager::p_instance = nullptr;

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

    WiFi.scanNetworksAsync(onScanCompleted);
}

void WiFiManager::disconnect()
{
    if (WiFi.status() != WL_CONNECTED)
        return;

    Serial.println("Disabling WiFi...");

    WiFi.disconnect(true);
}

void WiFiManager::onScanCompleted(int n)
{
    if (!n)
    {
        Serial.println("No networks found.");
        WiFi.disconnect(true);
        return;
    }

    p_instance->connectToKnownNetwork(n);
}

void WiFiManager::connectToKnownNetwork(int scannedCount)
{
    int bestRSSI = -999;
    String ssid;
    String password;

    m_storage->begin();
    m_storage->wifiFile()->open();

    for (int i = 0; i < scannedCount; i++)
    {
        String foundSSID = WiFi.SSID(i);
        int foundRSSI = WiFi.RSSI(i);
        auto foundPassword = m_storage->wifiFile()->getPassword(&foundSSID);
        bool isKnown = !foundPassword.isEmpty();

        Serial.printf("Found ");
        if (isKnown)
            Serial.printf("known ");
        Serial.printf("network: %s (RSSI %d)\n", foundSSID.c_str(), foundRSSI);

        if (!isKnown || foundRSSI <= bestRSSI)
            continue;

        ssid = foundSSID;
        password = foundPassword;

        bestRSSI = foundRSSI;
    }

    m_storage->wifiFile()->close();
    m_storage->end();

    if (ssid.isEmpty())
    {
        Serial.println("No known networks available.");
        WiFi.disconnect(true);
        return;
    }

    Serial.printf("Connecting to best network: %s\n", ssid.c_str());
    WiFi.begin(ssid, password);
}
