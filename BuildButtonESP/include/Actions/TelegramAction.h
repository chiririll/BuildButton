#ifndef TELEGRAM_ACTION_H
#define TELEGRAM_ACTION_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>

#include "Actions/WiFiAction.h"

#include "secrets.h"

class TelegramAction : WiFiAction
{
public:
    using WiFiAction::WiFiAction;

    void setPayload(const String *payload) override;

protected:
    void run_wifi() override;

private:
    String m_payload;
};

#endif // TELEGRAM_ACTION_H
