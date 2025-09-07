#ifndef NFC_H
#define NFC_H

#include <SPI.h>
#include <MFRC522.h>

class Nfc
{
public:
    Nfc(uint8_t rstPin, uint8_t sdaPin);

    bool loop();

private:
};

#endif // NFC_H
