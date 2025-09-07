#ifndef NFC_H
#define NFC_H

#include <Arduino.h>
#include <MFRC522.h>

#include "Systems/Speaker.h"

class Nfc
{
public:
    Nfc(uint8_t rstPin, uint8_t sdaPin);

    void init(Speaker *speaker);

    bool loop();

    void toggle();

    void begin(bool quiet = false);
    void end(bool quiet = false);

private:
    const uint enableSignal = 0b00000101;
    const uint disableSignal = 0b00001101;
    const uint readSignal = 0b00010101;

    const ulong startupDelay = 10;
    const ulong queryInterval = 100;
    const ulong enableDuration = 10000;

    MFRC522::MIFARE_Key m_key = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    Speaker *m_speaker;
    MFRC522 m_nfc;

    bool m_isActive;

    ulong m_nextQuery;
    ulong m_timeout;

    bool tryRead();
};

#endif // NFC_H
