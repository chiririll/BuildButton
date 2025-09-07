#ifndef SPEAKER_H
#define SPEAKER_H

#include <Arduino.h>

enum class SpeakerSignal
{
    None = 0,
    Beep = 0b00000001,
    DoubleBeep = 0b00000101,
    TripleBeep = 0b0010101,
    LongBeep = 0b00001111,
    ErrorTone = 0b01010101,

    OTAEnable = 0b00001011,
    OTADisable = 0b00101011,
};

class Speaker
{
public:
    Speaker(uint8_t pin);

    void speak(SpeakerSignal signal);

    bool loop();

private:
    const unsigned long beepDelay = 200;
    const unsigned long gapDelay = 150;

    uint8_t m_pin;

    uint8_t m_pattern;
    unsigned long m_nextToggle;
    bool m_isActive;
};

#endif // SPEAKER_H
