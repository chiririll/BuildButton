#include "Actions/CurlAction.h"

void CurlAction::setPayload(const String *payload)
{
    Serial.println("Loading payload for curl:");
    Serial.println(payload->c_str());
    // TODO: Parse
}

bool CurlAction::run_wifi()
{
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;

    if (m_user.length() > 0)
    {
        http.setAuthorization(m_user.c_str(), m_password.c_str());
    }
    if (m_contentType.length() > 0)
    {
        http.addHeader("Content-Type", m_contentType.c_str());
    }

    http.begin(client, m_user);

    int resultCode = http.POST(m_payload.c_str());

    if (resultCode > 0)
    {
        Serial.printf("Http result code: %d\n", resultCode);
        Serial.println(http.getString());
    }
    else
    {
        Serial.printf("Http request failed: %s\n", http.errorToString(resultCode).c_str());
    }

    http.end();
    return resultCode > 0;
}
