#include <Arduino.h>
#include <ESP8266WiFi.h>

#define SERIAL_FREQUENCY 115200

#define LED 4
#define BTN 5

bool printed = false;

void checkMessage(int buttonState)
{
    if (!buttonState && !printed)
    {
        Serial.println("Button!");
        printed = true;

        return;
    }

    if (buttonState && printed)
    {
        printed = false;
    }
}

void setupPins()
{
    pinMode(LED, OUTPUT);
    pinMode(BTN, INPUT_PULLUP);
}

void setupSerial()
{
    Serial.begin(SERIAL_FREQUENCY);
    Serial.println();

    Serial.println("Hello, World!");
}

void setupWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
}

String getEncType(uint8_t type)
{
    switch (type)
    {
    case ENC_TYPE_NONE:
        return "open";
    case ENC_TYPE_WEP:
        return "WEP";
    case ENC_TYPE_TKIP:
        return "TKIP";
    case ENC_TYPE_CCMP:
        return "CCMP";
    case ENC_TYPE_AUTO:
        return "auto";
    default:
        return String(type);
    }
}

void scanWiFi()
{
    Serial.println();

    int n = WiFi.scanNetworks();
    for (auto i = 0; i < n; i++)
    {
        Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), getEncType(WiFi.encryptionType(i)));
    }
}

void setup()
{
    setupPins();
    setupSerial();

    setupWiFi();
    scanWiFi();
}

void loop()
{
    auto buttonState = digitalRead(BTN);

    digitalWrite(LED, buttonState ? LOW : HIGH);

    checkMessage(buttonState);
}
