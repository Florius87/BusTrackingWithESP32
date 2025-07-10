#pragma once
#include <Arduino.h>

struct Departure {
  String line;
  String destination;
  String time;
  long minLeft;
};

void updateOVapi();
bool fetchOVapiPayload(const String& halteCode, String& payloadOut);
time_t parseISO8601(const String& iso);
void setupTime();
