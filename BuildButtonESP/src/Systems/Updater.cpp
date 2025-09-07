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
        stop();
    else
        start();
}

void Updater::start()
{
    m_wifi->setClient(WiFiClientType::Updater, true);

    ArduinoOTA.begin();

    m_speaker->speak(SpeakerSignal::OTAEnable);

    m_isActive = true;
}

void Updater::stop()
{
    ArduinoOTA.end();

    m_wifi->setClient(WiFiClientType::Updater, false);

    m_speaker->speak(SpeakerSignal::OTADisable);

    m_isActive = false;
}

bool Updater::loop()
{
    return false;
}
