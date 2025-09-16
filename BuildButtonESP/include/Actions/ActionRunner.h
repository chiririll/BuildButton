#ifndef RUNNER_H
#define RUNNER_H

#include "Actions/IAction.h"
#include "Actions/HttpPostAction.h"

#include "StorageManager.h"
#include "WiFiManager.h"

class ActionRunner
{
public:
    ActionRunner() : m_systems(), m_http(&m_systems) {}

    void init(WiFiManager *wifi, StorageManager *storage);
    bool loop();

    void run();
    void check_action();

private:
    ActionSystems m_systems;

    IAction *m_action;

    HttpPostAction m_http;
};

#endif // RUNNER_H
