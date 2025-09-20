#include "Files/ActionFile.h"

void ActionFile::save(const String *type, const String *payload)
{
    Serial.printf("Saving data for action %s\n", type->c_str());

    LittleFS.remove(actionFile);

    auto fAction = LittleFS.open(actionFile, "w");
    if (!fAction)
    {
        Serial.println("Failed to open action file!");
        return;
    }

    fAction.write(type->c_str());
    fAction.close();

    LittleFS.remove(payloadFile);

    auto fPayload = LittleFS.open(payloadFile, "w");
    if (!fPayload)
    {
        Serial.println("Failed to open payload file!");
        return;
    }

    fPayload.write(payload->c_str());
    fPayload.close();
}

const String ActionFile::getActiveAction()
{
    auto fAction = LittleFS.open(actionFile, "r");
    if (!fAction)
        return "";

    return fAction.readString();
}

const String ActionFile::getPayload()
{
    auto fPayload = LittleFS.open(payloadFile, "r");
    if (!fPayload)
        return "";

    return fPayload.readString();
}
