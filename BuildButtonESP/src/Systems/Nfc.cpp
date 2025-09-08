#include "Systems/NFC.h"

void Nfc::init(StorageManager *storage, Speaker *speaker)
{
    m_storage = storage;
    m_speaker = speaker;

    m_mfrc.PCD_Init();
    m_mfrc.PCD_SoftPowerDown();
}

void Nfc::begin(bool quiet)
{
    if (m_isActive)
        return;

    if (!quiet)
    {
        m_speaker->speak(enableSignal);
    }

    m_mfrc.PCD_SoftPowerUp();
    m_nfc.begin();

    auto now = millis();
    m_nextQuery = now + startupDelay;
    m_timeout = now + enableDuration;

    m_isActive = true;
}

void Nfc::end(bool quiet)
{
    if (!m_isActive)
        return;

    if (!quiet)
    {
        m_speaker->speak(disableSignal);
    }

    m_mfrc.PCD_SoftPowerDown();

    m_nextQuery = 0;
    m_timeout = 0;

    m_isActive = false;
}

void Nfc::toggle()
{
    if (m_isActive)
        end();
    else
        begin();
}

bool Nfc::loop()
{
    if (!m_isActive)
        return false;

    auto now = millis();
    if (now > m_timeout)
    {
        end();

        return false;
    }

    if (now < m_nextQuery)
    {
        return true;
    }

    if (tryRead())
    {
        m_speaker->speak(readSignal);
        end(true);

        return false;
    }

    m_nextQuery = now + queryInterval;
    return true;
}

bool Nfc::tryRead()
{
    if (!m_nfc.tagPresent())
        return false;

    NfcTag tag = m_nfc.read();

    if (!tag.hasNdefMessage())
        return false;

    NdefMessage message = tag.getNdefMessage();
    int recordCount = message.getRecordCount();

    for (int i = 0; i < recordCount; i++)
    {
        NdefRecord record = message.getRecord(i);

        switch (record.getTnf())
        {
        case NdefRecord::TNF::TNF_MIME_MEDIA:
            handleMediaRecord(&record);
            break;
        default:
            break;
        }
    }

    m_storage->wifiFile()->close();
    m_storage->end();

    return true;
}

void Nfc::handleMediaRecord(NdefRecord *record)
{
    auto length = record->getTypeLength();
    auto typeArr = record->getType();

    String type;
    for (uint i = 0; i < length; i++)
    {
        type += (char)typeArr[i];
    }

    if (type.equals("application/vnd.wfa.wsc"))
    {
        handleWiFiRecord(record);
        return;
    }

    if (type.startsWith("btn/"))
    {
        handleActionRecord(record);
        return;
    }

    if (type.startsWith("btnc/"))
    {
        handleCommandRecord(record);
        return;
    }
}

void Nfc::handleWiFiRecord(NdefRecord *record)
{
    auto payloadLength = record->getPayloadLength();
    auto payload = record->getPayload();

    WiFiRecord wifiRecord;
    wifiRecord.parse(payload, payloadLength);

    m_storage->begin();
    m_storage->wifiFile()->open();

    m_storage->wifiFile()->setPassword(wifiRecord.get_ssid(), wifiRecord.get_password());

    auto savedPassword = m_storage->wifiFile()->getPassword(wifiRecord.get_ssid());
    Serial.printf("Saved network: SSID='%s', Password='%s'\n", wifiRecord.get_ssid()->c_str(), savedPassword.c_str());
}

void Nfc::handleActionRecord(NdefRecord *record)
{
    Serial.println("Action not implemented");
}

void Nfc::handleCommandRecord(NdefRecord *record)
{
    Serial.println("Commands not implemented");
}