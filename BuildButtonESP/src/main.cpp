#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "WiFiCred.hpp"
#include "Jenkins.hpp"

// ==== Button setup ====
#define BUTTON_PIN 5 // GPIO0 (D3 on NodeMCU). Change if needed.
bool lastButtonState = HIGH;

void setup()
{
    Serial.begin(115200);
    delay(100);
    Serial.println();

    // Configure button
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Start WiFi
    WiFi.mode(WIFI_STA);
    connectToBestWiFi();
}

void loop()
{
    bool buttonState = digitalRead(BUTTON_PIN);
    if (lastButtonState == HIGH && buttonState == LOW)
    {
        Serial.println("Button pressed, triggering Jenkins build...");

        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("WiFi disconnected, rescanning...");
            connectToBestWiFi();
        }

        triggerJenkinsBuild();
        delay(5000);
    }

    lastButtonState = buttonState;
    delay(50); // debounce
}