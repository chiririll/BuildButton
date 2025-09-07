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
};

class Speaker
{
public:
    Speaker(uint8_t pin);

    void speak(SpeakerSignal signal) { speak((uint)signal); }
    void speak(uint pattern);

    bool loop();

private:
    const ulong beepDelay = 200;
    const ulong gapDelay = 150;

    uint8_t m_pin;

    uint m_pattern;
    ulong m_nextToggle;
    bool m_isActive;
};

#endif // SPEAKER_H
