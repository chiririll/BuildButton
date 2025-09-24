#include "Actions/TelegramAction.h"

void TelegramAction::setPayload(const String *payload)
{
    auto split = payload->indexOf('\n');

    auto chat_id = payload->substring(0, split);
    auto message = payload->substring(split + 1);

    m_payload = "{\"chat_id\":\"" + chat_id + "\",\"text\":\"" + message + "\"}";

    Serial.println("Telegram payload:");
    Serial.println(m_payload);
}

void TelegramAction::run_wifi()
{
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient https;
    String url = "https://api.telegram.org/bot" + TELEGRAM_TOKEN + "/sendMessage";

    https.begin(client, url);
    https.addHeader("Content-Type", "application/json");

    int httpCode = https.POST(m_payload);
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
