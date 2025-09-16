#ifndef MAIN_H
#define MAIN_H

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
#include "Actions/ActionRunner.h"

Sleep sleep;
StorageManager storage;
WiFiManager wifi;
Updater updater;

Button button(BUTTON_PIN);
Speaker speaker(SPEAKER_PIN);
Nfc nfc(SPI_RST_PIN, SPI_SDA_PIN);

ActionRunner runner;

void setup();
void loop();

void checkOverflow();

void updateSystems();
void checkActions();

#endif // MAIN_H