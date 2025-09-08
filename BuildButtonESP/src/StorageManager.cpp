#include "StorageManager.h"

bool StorageManager::begin()
{
    if (m_isActive)
        return true;

    m_isActive = SPIFFS.begin();
    return m_isActive;
}

void StorageManager::end()
{
    if (!m_isActive)
        return;

    SPIFFS.end();
    m_isActive = false;
}
