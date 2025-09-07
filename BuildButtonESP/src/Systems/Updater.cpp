#include "Systems/Updater.h"

void upd::onStart()
{
    Serial.println();
    Serial.println("Update started!");
}

void upd::onEnd()
{
    Serial.println();
    Serial.println("Update ended!");
}

void upd::onError(ota_error_t error)
{
    Serial.println();
    Serial.printf("Update error[%u]\n", error);
}

void Updater::init(WiFiManager *wifi, Speaker *speaker)
{
    m_wifi = wifi;
    m_speaker = speaker;

    ArduinoOTA.onStart(upd::onStart);
    ArduinoOTA.onEnd(upd::onEnd);
    ArduinoOTA.onError(upd::onError);
}

void Updater::toggle()
{
    if (m_isActive)
        end();
    else
        begin();
}

void Updater::begin()
{
    if (m_isActive)
        return;

    m_wifi->setClient(WiFiClientType::Updater, true);

    ArduinoOTA.begin();

    m_speaker->speak(enableSignal);

    m_isActive = true;
}

void Updater::end()
{
    if (!m_isActive)
        return;

    ArduinoOTA.end();

    m_wifi->setClient(WiFiClientType::Updater, false);

    m_speaker->speak(disableSignal);

    m_isActive = false;
}

bool Updater::loop()
{
    return false;
}
