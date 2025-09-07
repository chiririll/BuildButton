#include "Actions/JenkinsAction.h"

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
