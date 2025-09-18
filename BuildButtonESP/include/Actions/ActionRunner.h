#ifndef RUNNER_H
#define RUNNER_H

#include <Arduino.h>

#include "Actions/IAction.h"
#include "Actions/CurlAction.h"

#include "StorageManager.h"
#include "WiFiManager.h"

class ActionRunner
{
public:
    ActionRunner() : m_systems(), m_curl(&m_systems) {}

    void init(WiFiManager *wifi, StorageManager *storage, Speaker *speaker);
    bool loop();

    void run();
    void checkAction();

private:
    ActionSystems m_systems;

    IAction *m_action;

    CurlAction m_curl;

    void setAction(const String *type, const String *payload);
};

#endif // RUNNER_H
