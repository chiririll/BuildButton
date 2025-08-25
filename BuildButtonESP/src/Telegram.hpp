#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#ifndef TELEGRAM_H
#define TELEGRAM_H

const char *telegramHost = "api.telegram.org";
String botToken = "1969312819:AAE9TYamFlRV0UYgTDfYWoOksdr3c5viFn8";
String chatId = "534495936";

void sendTelegramMessage(const String &message)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi not connected!");
        return;
    }

    WiFiClientSecure client;
    client.setInsecure(); // ⚠️ ignore SSL certificate validation (ok for testing)

    HTTPClient https;

    String url = "https://" + String(telegramHost) + "/bot" + botToken + "/sendMessage";

    https.begin(client, url);
    https.addHeader("Content-Type", "application/json");

    String payload = "{\"chat_id\":\"" + chatId + "\",\"text\":\"" + message + "\"}";

    int httpCode = https.POST(payload);
    if (httpCode > 0)
    {
        Serial.printf("Telegram response code: %d\n", httpCode);
        Serial.println(https.getString());
    }
    else
    {
        Serial.printf("Telegram POST failed: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
}

#endif // TELEGRAM_H