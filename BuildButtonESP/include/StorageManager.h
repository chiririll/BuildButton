#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <LittleFS.h>

#include "Files/WiFiFile.h"

class StorageManager
{
public:
    bool begin();
    void end();

    WiFiFile *wifiFile() { return &m_wifiFile; }

private:
    bool m_isActive;

    WiFiFile m_wifiFile;
};

#endif // STORAGE_MANAGER_H
