#include "Actions/WiFiAction.h"

void WiFiAction::run()
{
    m_systems->m_wifi->setClient(WiFiClientType::ActionRunner, true);
    m_pending = true;
}

bool WiFiAction::loop()
{
    if (!m_pending)
        return false;

    if (WiFi.status() != WL_CONNECTED)
        return true;

    m_pending = false;

    if (run_wifi())
    {
        m_systems->m_speaker->speak(SpeakerSignal::Beep);
    }
    else
    {
        m_systems->m_speaker->speak(SpeakerSignal::ErrorTone);
    }

    m_systems->m_wifi->setClient(WiFiClientType::ActionRunner, false);
    return false;
}
