#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <LittleFS.h>

#include "Files/WiFiFile.h"
#include "Files/ActionFile.h"

class StorageManager
{
public:
    bool begin();
    void end();

    WiFiFile *wifiFile() { return &m_wifiFile; }
    ActionFile *actionFile() { return &m_actioFile; }

private:
    bool m_isActive;

    WiFiFile m_wifiFile;
    ActionFile m_actioFile;
};

#endif // STORAGE_MANAGER_H
