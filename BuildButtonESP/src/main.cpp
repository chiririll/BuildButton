#include "main.h"

void setup()
{
    Serial.begin(115200);
    Serial.println();

    SPI.begin();

    wifi.init(&storage);

    updater.init(&wifi, &speaker);
    nfc.init(&storage, &speaker, &runner);

    runner.init(&wifi, &storage, &speaker);
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

    isActive = runner.loop() || isActive;

    sleep.check(isActive);
}

void checkActions()
{
    switch (button.checkAction())
    {
    case ButtonAction::Tap:
        runner.run();
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
