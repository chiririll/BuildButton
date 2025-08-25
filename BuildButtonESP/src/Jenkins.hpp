#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#ifndef JENKINS_H
#define JENKINS_H

const char *jenkinsUrl = "https://jenkins.5c-team.com";

const char *jenkinsUser = "triggerbot";
const char *jenkinsToken = "11723cfecd6b45c9c23e1c686cd2c7b052";

String jobName = "PokeCapyGo";

void triggerJenkinsBuild()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi not connected!");
        return;
    }

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient request;

    String url = String(jenkinsUrl) + "/job/" + jobName + "/buildWithParameters";

    request.setAuthorization(jenkinsUser, jenkinsToken);
    request.begin(client, url);

    int httpCode = request.POST("");
    if (httpCode > 0)
    {
        Serial.printf("Jenkins response code: %d\n", httpCode);
        Serial.println(request.getString());
    }
    else
    {
        Serial.printf("Jenkins request failed: %s\n", request.errorToString(httpCode).c_str());
    }

    request.end();
}

#endif