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
    static const ulong debounceDelay = 50;
    static const ulong holdTime = 1200;
    static const ulong multiTapGap = 600;
    static const ulong cooldownTime = 2000;

    uint8_t m_pin;

    bool m_lastState;

    ulong m_pressStart;
    ulong m_nextTime;
    ulong m_resetTime;

    uint8_t m_tapCount;

    ButtonAction decode();
    ButtonAction decodeAndReset();
    void reset();
};

#endif // BUTTON_H
