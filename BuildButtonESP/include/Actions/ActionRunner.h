#ifndef RUNNER_H
#define RUNNER_H

#include <Arduino.h>

#include "Actions/BaseAction.h"
#include "Actions/CurlAction.h"
#include "Actions/TelegramAction.h"
#include "Actions/JenkinsAction.h"

#include "StorageManager.h"
#include "WiFiManager.h"

class ActionRunner
{
public:
    ActionRunner() : m_systems(), m_curl(&m_systems), m_telegram(&m_systems), m_jenkins(&m_systems) {}

    void init(WiFiManager *wifi, StorageManager *storage, Speaker *speaker);
    bool loop();

    void run();
    void checkAction();

private:
    ActionSystems m_systems;

    BaseAction *m_action;

    CurlAction m_curl;
    TelegramAction m_telegram;
    JenkinsAction m_jenkins;

    void setAction(const String *type, const String *payload);
    BaseAction *getAction(const String *type);
};

#endif // RUNNER_H
