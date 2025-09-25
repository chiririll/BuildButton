#ifndef WIFI_ACTION_H
#define WIFI_ACTION_H

#include <ESP8266HTTPClient.h>

#include "Actions/BaseAction.h"

class WiFiAction : BaseAction
{
public:
    using BaseAction::BaseAction;

    void run() override;
    bool loop() override;

    virtual void setPayload(const String *payload) override = 0;

protected:
    virtual bool run_wifi() = 0;

private:
    bool m_pending;
};

#endif // WIFI_ACTION_H