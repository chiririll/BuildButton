#ifndef WIFI_RECORD_H
#define WIFI_RECORD_H

#include <Arduino.h>

enum class WiFiRecordAttribute
{
    CREDENTIAL = 0x100E,
    SSID = 0x1045,
    PASSWORD = 0x1027,
    AUTHENTICATION_TYPE = 0x1003,
    ENCRYPTION_TYPE = 0x100F,
};

class WiFiRecord
{
public:
    String *get_ssid() { return &m_ssid; }
    String *get_password() { return &m_password; }

    void parse(const byte *payload, int length, int offset = 0);

private:
    String m_ssid;
    String m_password;
};

#endif // WIFI_RECORD_H