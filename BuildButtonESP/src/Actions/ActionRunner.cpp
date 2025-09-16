#include "Actions/ActionRunner.h"

void ActionRunner::init(WiFiManager *wifi, StorageManager *storage)
{
    m_systems.init(wifi, storage);
}

bool ActionRunner::loop()
{
    return m_action != NULL && m_action->loop();
}

void ActionRunner::run()
{
    if (m_action == NULL)
        return;
}

void ActionRunner::check_action()
{
    m_systems.m_storage->begin();
    m_systems.m_storage->end();
}
