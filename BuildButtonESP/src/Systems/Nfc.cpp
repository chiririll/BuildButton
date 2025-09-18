#include "Systems/NFC.h"

void Nfc::init(StorageManager *storage, Speaker *speaker, ActionRunner *runner)
{
    m_storage = storage;
    m_speaker = speaker;
    m_runner = runner;

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

    Serial.println("NFC Module enabled");

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

    Serial.println("NFC Module disabled");

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

    auto tag = m_nfc.read();
    Serial.println("Found tag: " + tag.getUidString());

    if (!tag.hasNdefMessage())
    {
        m_nfc.haltTag();
        return false;
    }

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
    auto type = String((char *)record->getType());

    if (type.startsWith("application/vnd.wfa.wsc"))
    {
        handleWiFiRecord(record);
        return;
    }

    if (type.startsWith("btn/"))
    {
        const int lPrefix = 4;
        auto actionType = type.substring(lPrefix, record->getTypeLength());
        handleActionRecord(&actionType, record);
        return;
    }

    if (type.startsWith("btnc/"))
    {
        const int lPrefix = 5;
        auto commandType = type.substring(lPrefix, record->getTypeLength());
        handleCommandRecord(&commandType, record);
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
    Serial.printf("Saved network: SSID='%s', Password='%s', (%d total)\n",
                  wifiRecord.get_ssid()->c_str(),
                  savedPassword.c_str(),
                  m_storage->wifiFile()->get_networksCount());

    m_storage->wifiFile()->close();
    m_storage->end();
}

void Nfc::handleActionRecord(const String *type, NdefRecord *record)
{
    m_storage->begin();

    auto payload = String((char *)record->getPayload());
    payload.substring(0, record->getPayloadLength());

    m_storage->actionFile()->save(type, &payload);

    m_runner->checkAction();

    m_storage->end();
}

void Nfc::handleCommandRecord(const String *type, NdefRecord *record)
{
    Serial.println("Command " + *type + " is not implemented");
}
