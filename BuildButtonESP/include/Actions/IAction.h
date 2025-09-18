#ifndef IACTION_H
#define IACTION_H

#include <Arduino.h>

#include "WiFiManager.h"
#include "StorageManager.h"

#include "Systems/Speaker.h"

class ActionSystems
{
public:
    WiFiManager *m_wifi;
    StorageManager *m_storage;

    Speaker *m_speaker;

    void init(WiFiManager *wifi, StorageManager *storage, Speaker *speaker)
    {
        m_wifi = wifi;
        m_storage = storage;
        m_speaker = speaker;
    }
};

class IAction
{
public:
    IAction(ActionSystems *systems) : m_systems(systems) {}

    virtual void run() = 0;
    virtual bool loop() = 0;

    virtual void setPayload(const String *payload) = 0;

protected:
    ActionSystems *m_systems;
};

#endif // IACTION_H
