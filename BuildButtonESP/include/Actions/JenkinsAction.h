#ifndef JENKINS_ACTION_H
#define JENKINS_ACTION_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>

#include "Actions/WiFiAction.h"

#include "secrets.h"

class JenkinsAction : WiFiAction
{
public:
    using WiFiAction::WiFiAction;

    void setPayload(const String *payload) override;

protected:
    void run_wifi() override;
};

#endif // JENKINS_ACTION_H
