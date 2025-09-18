#ifndef NFC_H
#define NFC_H

#include <Arduino.h>
#include <MFRC522.h>
#include <NfcAdapter.h>

#include "StorageManager.h"
#include "Records/WiFiRecord.h"

#include "Actions/ActionRunner.h"
#include "Systems/Speaker.h"

class Nfc
{
public:
    Nfc(uint8_t rstPin, uint8_t sdaPin) : m_mfrc(rstPin, sdaPin), m_nfc(&m_mfrc) {}

    void init(StorageManager *storage, Speaker *speaker, ActionRunner *runner);

    bool loop();

    void toggle();

    void begin(bool quiet = false);
    void end(bool quiet = false);

private:
    const uint enableSignal = 0b00000101;
    const uint disableSignal = 0b00001101;
    const uint readSignal = 0b00010101;

    const ulong startupDelay = 10;
    const ulong queryInterval = 200;
    const ulong enableDuration = 10000;

    StorageManager *m_storage;
    Speaker *m_speaker;
    ActionRunner *m_runner;

    MFRC522 m_mfrc;
    NfcAdapter m_nfc;

    bool m_isActive;

    ulong m_nextQuery;
    ulong m_timeout;

    bool tryRead();

    void handleMediaRecord(NdefRecord *record);

    void handleWiFiRecord(NdefRecord *record);
    void handleActionRecord(const String *type, NdefRecord *record);
    void handleCommandRecord(const String *type, NdefRecord *record);
};

#endif // NFC_H
