#include "Actions/TelegramAction.h"

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
