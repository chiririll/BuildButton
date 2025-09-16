#ifndef IACTION_H
#define IACTION_H

#include "WiFiManager.h"
#include "StorageManager.h"

class ActionSystems
{
public:
    WiFiManager *m_wifi;
    StorageManager *m_storage;

    void init(WiFiManager *wifi, StorageManager *storage)
    {
        m_wifi = wifi;
        m_storage = storage;
    }
};

class IAction
{
public:
    IAction(ActionSystems *systems) : m_systems(systems) {}

    virtual void run() = 0;
    virtual bool loop() = 0;

protected:
    ActionSystems *m_systems;
};

#endif // IACTION_H
