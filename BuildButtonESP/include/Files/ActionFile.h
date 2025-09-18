#ifndef ACTION_FILE_H
#define ACTION_FILE_H

#include <Arduino.h>
#include <LittleFS.h>

class ActionFile
{
public:
    void save(const String *type, const String *payload);

    const String getActiveAction();
    const String getPayload();

private:
    const char *actionFile = "/action.dat";
    const char *payloadFile = "/payload.dat";
};

#endif // ACTION_FILE_H