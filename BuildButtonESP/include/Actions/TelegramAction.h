#ifndef TELEGRAM_ACTION_H
#define TELEGRAM_ACTION_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *telegramHost = "api.telegram.org";
String botToken = "1969312819:AAE9TYamFlRV0UYgTDfYWoOksdr3c5viFn8";
String chatId = "534495936";

void sendTelegramMessage(const String &message);

#endif // TELEGRAM_ACTION_H
