#include "Records/WiFiRecord.h"

void WiFiRecord::parse(const byte *payload, int length, int i)
{
    while (i + 4 <= length)
    {
        uint16_t attrId = (payload[i] << 8) | payload[i + 1];
        uint16_t len = (payload[i + 2] << 8) | payload[i + 3];
        i += 4;

        if (i + len > length)
            break;

        switch ((WiFiRecordAttribute)attrId)
        {
        case WiFiRecordAttribute::CREDENTIAL:
        {
            parse(payload, length, i);
            break;
        }
        case WiFiRecordAttribute::SSID:
        {
            m_ssid = "";
            for (int j = 0; j < len; j++)
                m_ssid += (char)payload[i + j];
            break;
        }
        case WiFiRecordAttribute::PASSWORD:
        {
            m_password = "";
            for (int j = 0; j < len; j++)
                m_password += (char)payload[i + j];
            break;
        }
        default:
            break;
        }

        i += len;
    }
}
