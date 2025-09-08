#include "Files/WiFiFile.h"

void WiFiFile::open()
{
    if (m_isOpened)
        return;

    m_file = LittleFS.open(filename, "w+");

    char buffer[1];
    auto readCount = m_file.readBytes(buffer, sizeof(buffer));

    if (!readCount)
    {
        m_file.seek(0);
        m_file.write(0);
    }

    m_networksCount = readCount > 0 ? (uint8_t)buffer[0] : 0;
    m_isOpened = true;
}

void WiFiFile::close()
{
    if (!m_isOpened)
        return;

    m_file.close();

    m_networksCount = 0;
    m_isOpened = false;
}

String WiFiFile::getPassword(const String *ssid)
{
    if (!m_networksCount)
        return "";

    m_file.seek(1);
    for (size_t i = 0; i < m_networksCount; i++)
    {
        auto i_ssid = m_file.readStringUntil('\0');
        m_file.seek(1, SeekMode::SeekCur);
        auto i_password = m_file.readStringUntil('\0');
        m_file.seek(1, SeekMode::SeekCur);

        if (!i_ssid.equals(*ssid))
            continue;

        return i_password;
    }

    return "";
}

void WiFiFile::setPassword(const String *ssid, const String *password)
{
    if (!m_networksCount)
        return;

    if (getPassword(ssid))
        return;

    m_file.seek(0, SeekMode::SeekEnd);

    m_file.write(ssid->c_str());
    m_file.write("\0");
    m_file.write(password->c_str());
    m_file.write("\0");

    m_networksCount++;

    m_file.seek(0);
    m_file.write(m_networksCount);
}
