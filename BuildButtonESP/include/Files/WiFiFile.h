#ifndef WIFI_FILE_H
#define WIFI_FILE_H

#include <Arduino.h>
#include <LittleFS.h>

class WiFiFile
{
public:
    void open();
    void close();

    int get_networksCount() { return m_networksCount; }

    String getPassword(const String *ssid);
    void setPassword(const String *ssid, const String *password);

private:
    const char *filename = "/wifi.dat";

    bool m_isOpened;

    fs::File m_file;

    size_t m_networksCount;
};

#endif // WIFI_FILE_H