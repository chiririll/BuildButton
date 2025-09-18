#include "Actions/CurlAction.h"

void CurlAction::run()
{
    m_systems->m_wifi->setClient(WiFiClientType::ActionRunner, true);
    m_pending = true;
}

bool CurlAction::loop()
{
    if (!m_pending)
        return false;

    if (WiFi.status() != WL_CONNECTED)
        return true;

    m_pending = false;
    send();
    m_systems->m_wifi->setClient(WiFiClientType::ActionRunner, false);

    return false;
}

void CurlAction::setPayload(const String *payload)
{
    // TODO: Parse
}

void CurlAction::send()
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
}
