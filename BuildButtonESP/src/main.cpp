#include <Arduino.h>
#include <Esp.h>
#include <SPI.h>

#include "Pins.h"
#include "WiFiManager.h"

#include "Systems/Sleep.h"
#include "Systems/Updater.h"

#include "Systems/Button.h"
#include "Systems/Speaker.h"
#include "Systems/NFC.h"

Sleep sleep;
StorageManager storage;
WiFiManager wifi;
Updater updater;

Button button(BUTTON_PIN);
Speaker speaker(SPEAKER_PIN);
Nfc nfc(SPI_RST_PIN, SPI_SDA_PIN);

void checkOverflow();

void updateSystems();
void checkActions();

void setup()
{
    Serial.begin(115200);
    Serial.println();

    SPI.begin();

    wifi.init(&storage);

    updater.init(&wifi, &speaker);
    nfc.init(&storage, &speaker);
}

void loop()
{
    checkOverflow();

    updateSystems();
    checkActions();
}

void checkOverflow()
{
    if (millis() < 2e18)
        return;

    ESP.restart();
}

void updateSystems()
{
    bool isActive = false;

    isActive = wifi.loop() || isActive;
    isActive = speaker.loop() || isActive;

    isActive = updater.loop() || isActive;
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
        nfc.toggle();
        break;
    case ButtonAction::Hold:
        updater.toggle();
        break;
    default:
        return;
    }

    sleep.wake();
}
