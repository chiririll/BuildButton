#ifndef UPDATER_H
#define UPDATER_H

#include <Arduino.h>
#include <ArduinoOTA.h>

#include "WiFiManager.h"
#include "Systems/Speaker.h"

class Updater
{
public:
    void init(WiFiManager *wifi, Speaker *speaker);

    bool loop();

    void toggle();

    void start();
    void stop();

private:
    WiFiManager *m_wifi;
    Speaker *m_speaker;

    bool m_isActive;
};

namespace upd
{
    void onStart();
    void onEnd();
    void onError(ota_error_t error);
};

#endif // UPDATER_H
