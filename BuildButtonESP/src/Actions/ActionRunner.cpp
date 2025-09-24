#include "Actions/ActionRunner.h"

void ActionRunner::init(WiFiManager *wifi, StorageManager *storage, Speaker *speaker)
{
    m_systems.init(wifi, storage, speaker);

    checkAction();
}

bool ActionRunner::loop()
{
    return m_action != NULL && m_action->loop();
}

void ActionRunner::run()
{
    if (m_action == NULL)
    {
        Serial.println("No active action selected");
        m_systems.m_speaker->speak(SpeakerSignal::LongBeep);
        return;
    }

    m_systems.m_speaker->speak(SpeakerSignal::Beep);

    m_action->run();
}

void ActionRunner::checkAction()
{
    Serial.println("Updating active action...");

    m_systems.m_storage->begin();

    auto action = m_systems.m_storage->actionFile()->getActiveAction();
    auto payload = m_systems.m_storage->actionFile()->getPayload();

    m_action = NULL;
    setAction(&action, &payload);

    if (!m_action)
    {
        Serial.printf("Selected action '%s' is unknown!\n", action.c_str());
    }
    else
    {
        Serial.printf("Selected action '%s' is loaded!\n", action.c_str());
    }

    m_systems.m_storage->end();
}

void ActionRunner::setAction(const String *type, const String *payload)
{
    m_action = getAction(type);

    if (!m_action)
        return;

    m_action->setPayload(payload);
}

BaseAction *ActionRunner::getAction(const String *type)
{
    if (type->startsWith("curl"))
        return (BaseAction *)&m_curl;

    if (type->startsWith("telegram"))
        return (BaseAction *)&m_telegram;

    if (type->startsWith("jenkins"))
        return (BaseAction *)&m_jenkins;

    return NULL;
}
