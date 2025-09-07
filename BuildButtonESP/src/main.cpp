#include <Arduino.h>

#include "Pins.h"

#include "Systems/Sleep.h"

#include "Systems/Button.h"
#include "Systems/Speaker.h"
#include "Systems/NFC.h"

Sleep sleep;

Button button(BUTTON_PIN);
Speaker speaker(SPEAKER_PIN);
Nfc nfc(SPI_RST_PIN, SPI_SDA_PIN);

void updateSystems();
void checkActions();

void setup()
{
    Serial.begin(115200);
    Serial.println();
}

void loop()
{
    updateSystems();
    checkActions();
}

void updateSystems()
{
    bool isActive = false;

    isActive = speaker.loop() || isActive;
    isActive = nfc.loop() || isActive;

    sleep.check(isActive);
}

void checkActions()
{
    switch (button.checkAction())
    {
    case ButtonAction::Tap:
        speaker.speak(SpeakerSignal::Beep);
        // TODO: Start action
        break;
    case ButtonAction::TripleTap:
        speaker.speak(SpeakerSignal::TripleBeep);
        // TODO: Start action
        break;
    case ButtonAction::Hold:
        speaker.speak(SpeakerSignal::LongBeep);
        // TODO: Start action
        break;
    default:
        return;
    }

    sleep.wake();
}
