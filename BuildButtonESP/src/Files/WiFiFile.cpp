#include "Files/WiFiFile.h"

void WiFiFile::open()
{
    if (m_isOpened)
        return;

    m_file = LittleFS.open(filename, "w+");
    if (!m_file)
        return;

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

    m_file.seek(1, SeekMode::SeekSet);
    for (size_t i = 0; i < m_networksCount; i++)
    {
        auto i_ssid = m_file.readStringUntil('\0');
        auto i_password = m_file.readStringUntil('\0');

        if (!i_ssid.equals(*ssid))
            continue;

        return i_password;
    }

    return "";
}

void WiFiFile::setPassword(const String *ssid, const String *password)
{
    if (!m_isOpened)
        return;

    if (!getPassword(ssid).isEmpty())
        return;

    m_file.seek(0, SeekMode::SeekEnd);

    m_file.write(ssid->c_str(), ssid->length() + 1);
    m_file.write(password->c_str(), password->length() + 1);

    m_networksCount++;

    m_file.seek(0, SeekMode::SeekSet);
    m_file.write(m_networksCount);
}
