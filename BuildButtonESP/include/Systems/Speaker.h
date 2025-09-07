#ifndef SPEAKER_H
#define SPEAKER_H

#include <Arduino.h>

enum class SpeakerSignal
{
    None = 0,
    Beep = 0b00000001,       // short beep
    DoubleBeep = 0b00000101, // two short beeps
    TripleBeep = 0b0010101,  // three short beeps
    LongBeep = 0b00001111,   // sustained beep
    ErrorTone = 0b01010101,  // fast repeated beeps
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
