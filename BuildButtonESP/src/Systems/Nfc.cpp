#include "Systems/NFC.h"

Nfc::Nfc(uint8_t rstPin, uint8_t sdaPin) : m_mfrc(rstPin, sdaPin), m_nfc(&m_mfrc)
{
}

void Nfc::init(Speaker *speaker)
{
    m_speaker = speaker;

    m_mfrc.PCD_Init();
    m_mfrc.PCD_SoftPowerDown();
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

void printHexChar(const byte *arr, const uint length)
{
    for (int i = 0; i < length; i++)
    {
        Serial.print(arr[i], HEX);
        Serial.print(" ");
    }
}

bool Nfc::tryRead()
{
    if (!m_nfc.tagPresent())
        return false;

    Serial.println("Reading NFC tag");
    NfcTag tag = m_nfc.read();

    Serial.println(tag.getTagType());
    Serial.print("UID: ");
    Serial.println(tag.getUidString());

    if (!tag.hasNdefMessage())
        return false;

    NdefMessage message = tag.getNdefMessage();

    Serial.print("\nThis NFC Tag contains an NDEF Message with ");
    Serial.print(message.getRecordCount());
    Serial.print(" NDEF Record");

    if (message.getRecordCount() != 1)
    {
        Serial.print("s");
    }

    Serial.println(".");

    // cycle through the records, printing some info from each
    int recordCount = message.getRecordCount();
    for (int i = 0; i < recordCount; i++)
    {
        Serial.print("\nNDEF Record ");
        Serial.println(i + 1);
        NdefRecord record = message.getRecord(i);
        // NdefRecord record = message[i]; // alternate syntax

        Serial.print("  TNF: ");
        Serial.println(record.getTnf());

        Serial.print("  Type: ");
        printHexChar(record.getType(), record.getTypeLength()); // will be "" for TNF_EMPTY
        Serial.println();

        // The TNF and Type should be used to determine how your application processes the payload
        // There's no generic processing for the payload, it's returned as a byte[]
        int payloadLength = record.getPayloadLength();
        const byte *payload = record.getPayload();

        // Print the Hex and Printable Characters
        Serial.print("  Payload (HEX): ");
        printHexChar(payload, payloadLength);
        Serial.println();

        // Force the data into a String (might work depending on the content)
        // Real code should use smarter processing
        String payloadAsString = "";
        for (int c = 0; c < payloadLength; c++)
        {
            payloadAsString += (char)payload[c];
        }
        Serial.print("  Payload (as String): ");
        Serial.println(payloadAsString);

        // id is probably blank and will return ""
        if (record.getIdLength() > 0)
        {
            Serial.print("  ID: ");
            printHexChar(record.getId(), record.getIdLength());
            Serial.println();
        }
    }

    return true;
}

void Nfc::toggle()
{
    if (m_isActive)
        end();
    else
        begin();
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
