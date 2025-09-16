#ifndef HTTP_POST_ACTION_H
#define HTTP_POST_ACTION_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "Actions/IAction.h"

class HttpPostAction : IAction
{
public:
    using IAction::IAction;

    void run() override;
    bool loop() override;

private:
    bool m_pending;

    String m_url;
    String m_contentType;
    String m_user;
    String m_password;
    String m_payload;

    void send();
};

#endif // HTTP_POST_ACTION_H
