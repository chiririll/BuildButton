#include <Arduino.h>
#include <ESP8266WiFi.h>

#ifndef WIFICRED_H
#define WIFICRED_H

// ==== Known WiFi networks (add as many as you want) ====
struct WiFiCred
{
    const char *ssid;
    const char *password;
};

WiFiCred wifiNetworks[] = {
    {"wifi", "*1*2*3*4*"},
    {"BB", "ah6haRcmcX"},
};

const int wifiCount = sizeof(wifiNetworks) / sizeof(wifiNetworks[0]);

// ==== Connect to WiFi ====
void connectToWiFi()
{
    for (int i = 0; i < wifiCount; i++)
    {
        Serial.printf("Trying SSID: %s\n", wifiNetworks[i].ssid);
        WiFi.begin(wifiNetworks[i].ssid, wifiNetworks[i].password);

        unsigned long startAttempt = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 8000)
        {
            delay(500);
            Serial.print(".");
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.printf("\nConnected to %s\n", wifiNetworks[i].ssid);
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
            return;
        }
        Serial.println("\nFailed, trying next...");
    }
    Serial.println("No known WiFi networks available.");
}

// ==== Connect to strongest known WiFi ====
void connectToBestWiFi()
{
    Serial.println("Scanning for WiFi networks...");
    int n = WiFi.scanNetworks();
    if (n == 0)
    {
        Serial.println("No networks found.");
        return;
    }

    int bestNetIndex = -1;
    int bestRSSI = -999;

    for (int i = 0; i < n; i++)
    {
        String foundSSID = WiFi.SSID(i);
        int foundRSSI = WiFi.RSSI(i);

        Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");

        for (int k = 0; k < wifiCount; k++)
        {
            if (foundSSID != wifiNetworks[k].ssid)
                continue;

            Serial.printf("Found known network: %s (RSSI %d)\n", foundSSID.c_str(), foundRSSI);

            if (foundRSSI <= bestRSSI)
                continue;

            bestRSSI = foundRSSI;
            bestNetIndex = k;
        }
    }

    if (bestNetIndex < 0)
    {
        Serial.println("No known networks available.");
        return;
    }

    Serial.printf("Connecting to best network: %s\n", wifiNetworks[bestNetIndex].ssid);
    WiFi.begin(wifiNetworks[bestNetIndex].ssid, wifiNetworks[bestNetIndex].password);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000)
    {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("\nFailed to connect.");
        return;
    }

    Serial.printf("\nConnected! IP: %s\n", WiFi.localIP().toString().c_str());
}

#endif // WIFICRED_H