#include "Systems/Button.h"

Button::Button(uint8_t pin) : m_pin(pin)
{
    pinMode(m_pin, INPUT_PULLUP);
    m_lastState = digitalRead(m_pin);
}

ButtonAction Button::checkAction()
{
    auto now = millis();

    if (now < m_nextTime)
        return ButtonAction::None;

    bool state = digitalRead(m_pin);

    if (state == m_lastState)
    {
        if (m_pressStart == 0)
            return ButtonAction::None;

        if (state == LOW && now > m_pressStart + holdTime)
            return decodeAndReset();

        if (state == HIGH && now > m_resetTime)
            return decodeAndReset();

        return ButtonAction::None;
    }

    if (state == LOW)
    {
        m_pressStart = now;
    }
    else
    {
        m_tapCount++;
        m_resetTime = now + multiTapGap;
    }

    m_nextTime = now + debounceDelay;

    m_lastState = state;
    return ButtonAction::None;
}

ButtonAction Button::decodeAndReset()
{
    auto action = decode();
    reset();
    return action;
}

ButtonAction Button::decode()
{
    // Ends with hold
    if (!m_lastState && millis() > m_pressStart + holdTime)
        return ButtonAction::Hold;

    if (m_tapCount == 1)
        return ButtonAction::Tap;
    if (m_tapCount == 2)
        return ButtonAction::DoubleTap;
    if (m_tapCount == 3)
        return ButtonAction::TripleTap;

    return ButtonAction::None;
}

void Button::reset()
{
    m_pressStart = 0;
    m_resetTime = 0;
    m_tapCount = 0;

    m_nextTime = 0;
    m_lastState = digitalRead(m_pin);
}
