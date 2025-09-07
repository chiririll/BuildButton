#include "Systems/NFC.h"

Nfc::Nfc(uint8_t rstPin, uint8_t sdaPin) : m_nfc(rstPin, sdaPin)
{
}

void Nfc::init(Speaker *speaker)
{
    m_speaker = speaker;

    m_nfc.PCD_Init();
    m_nfc.PCD_SoftPowerDown();
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
    if (!m_nfc.PICC_IsNewCardPresent())
        return false;

    if (!m_nfc.PICC_ReadCardSerial())
        return false;

    byte buffer[18];
    byte size = sizeof(buffer);

    for (byte sector = 1; sector < 16; sector++)
    {
        Serial.print("\nSector ");
        Serial.print(sector);
        Serial.println(":");

        byte firstBlock = sector * 4;
        for (byte block = firstBlock; block < firstBlock + 3; block++)
        {
            // Authenticate with Key B first (your card seems to use it)
            MFRC522::StatusCode status = m_nfc.PCD_Authenticate(
                MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &m_key, &(m_nfc.uid));

            if (status != MFRC522::STATUS_OK)
            {
                Serial.print("Auth failed for block ");
                Serial.print(block);
                Serial.print(": ");
                Serial.println(m_nfc.GetStatusCodeName(status));
                continue;
            }

            // Read the block
            status = m_nfc.MIFARE_Read(block, buffer, &size);
            if (status != MFRC522::STATUS_OK)
            {
                Serial.print("Read failed for block ");
                Serial.print(block);
                Serial.print(": ");
                Serial.println(m_nfc.GetStatusCodeName(status));
                continue;
            }

            // Check if block is "non-zero"
            bool nonZero = false;
            for (byte i = 0; i < 16; i++)
            {
                if (buffer[i] != 0x00)
                {
                    nonZero = true;
                    break;
                }
            }

            if (nonZero)
            {
                Serial.print("Block ");
                Serial.print(block);
                Serial.print(": ");
                for (byte i = 0; i < 16; i++)
                {
                    if (isPrintable(buffer[i]))
                        Serial.write(buffer[i]);
                    else
                        Serial.print(".");
                }
                Serial.println();
            }
        }
    }

    m_nfc.PICC_HaltA();
    m_nfc.PCD_StopCrypto1();

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

    m_nfc.PCD_SoftPowerUp();

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

    m_nfc.PCD_SoftPowerDown();

    m_nextQuery = 0;
    m_timeout = 0;

    m_isActive = false;
}
