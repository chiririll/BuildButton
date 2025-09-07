#ifndef JENKINS_ACTION_H
#define JENKINS_ACTION_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *jenkinsUrl = "https://jenkins.5c-team.com";

const char *jenkinsUser = "triggerbot";
const char *jenkinsToken = "11723cfecd6b45c9c23e1c686cd2c7b052";

String jobName = "PokeCapyGo";

void triggerJenkinsBuild();

#endif // JENKINS_ACTION_H