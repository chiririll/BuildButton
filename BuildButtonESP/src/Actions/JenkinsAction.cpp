#include "Actions/JenkinsAction.h"

void JenkinsAction::setPayload(const String *payload)
{
    auto split = payload->indexOf('\n');

    m_projectName = payload->substring(0, split);
    m_parameters = payload->substring(split + 1);

    Serial.println("Jenkins payload:");

    Serial.print("Project: ");
    Serial.println(m_projectName);

    Serial.print("Parameters: ");
    Serial.println(m_parameters.length() > 0 ? m_parameters : "<none>");
}

void JenkinsAction::run_wifi()
{
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient https;
    https.setAuthorization(JENKINS_USER, JENKINS_PASSWORD);

    String url = JENKINS_URL + "job/" + m_projectName + (m_parameters.length() > 0 ? "buildWithParameters" : "build");
    https.begin(client, url);

    int httpCode = https.POST("");
    if (httpCode > 0)
    {
        Serial.printf("Jenkins code: %d\n", httpCode);
        Serial.println(https.getString());
    }
    else
    {
        Serial.printf("Jenkins request failed: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
}
