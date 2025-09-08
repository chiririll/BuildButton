#include "StorageManager.h"

bool StorageManager::begin()
{
    if (m_isActive)
        return true;

    m_isActive = LittleFS.begin();
    return m_isActive;
}

void StorageManager::end()
{
    if (!m_isActive)
        return;

    LittleFS.end();
    m_isActive = false;
}
