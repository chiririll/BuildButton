#ifndef CURL_ACTION_H
#define CURL_ACTION_H

#include <Arduino.h>

#include "Actions/WiFiAction.h"

class CurlRequestData
{
public:
    enum class RequestType
    {
        GET,
        POST,
    };

public:
    RequestType type;

    String url;
    String payload;
};

class CurlAction : WiFiAction
{
public:
    using WiFiAction::WiFiAction;

    void setPayload(const String *payload) override;

protected:
    bool run_wifi() override;

private:
    String m_url;
    String m_contentType;
    String m_user;
    String m_password;
    String m_payload;
};

#endif // CURL_ACTION_H
