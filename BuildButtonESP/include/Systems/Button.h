#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

enum class ButtonAction
{
    None = 0,
    Tap,
    DoubleTap,
    TripleTap,
    Hold,
};

class Button
{
public:
    Button(uint8_t pin);

    ButtonAction checkAction();

private:
    static const unsigned long debounceDelay = 50;
    static const unsigned long holdTime = 1200;
    static const unsigned long multiTapGap = 600;
    static const unsigned long cooldownTime = 2000;

    uint8_t m_pin;

    bool m_lastState;

    unsigned long m_pressStart;
    unsigned long m_nextTime;
    unsigned long m_resetTime;

    uint8_t m_tapCount;

    ButtonAction decode();
    ButtonAction decodeAndReset();
    void reset();
};

#endif // BUTTON_H
