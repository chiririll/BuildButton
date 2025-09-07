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

    void begin();
    void end();

private:
    const uint enableSignal = 0b00001011;
    const uint disableSignal = 0b00101011;

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
