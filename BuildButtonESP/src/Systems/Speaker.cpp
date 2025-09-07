#include "Systems/Speaker.h"

Speaker::Speaker(uint8_t pin) : m_pin(pin)
{
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, HIGH);
}

void Speaker::speak(SpeakerSignal signal)
{
    m_pattern = (int)signal;

    m_isActive = true;
    m_nextToggle = millis();

    digitalWrite(m_pin, HIGH);
}

bool Speaker::loop()
{
    if (!m_isActive)
        return false;

    auto now = millis();

    if (now < m_nextToggle)
        return true;

    if (m_pattern == 0)
    {
        digitalWrite(m_pin, HIGH);
        m_isActive = false;

        return false;
    }

    auto state = m_pattern & 1;

    digitalWrite(m_pin, state ? LOW : HIGH);
    m_pattern >>= 1;

    m_nextToggle = now + (state ? beepDelay : gapDelay);
    return true;
}
