#pragma once
#include <Arduino.h>
#include "ovapi.h"


void printLCD(const String& halteNaam, const char* tijdStr, Departure departures[], int depCount);
String shortenHalteName(const String& fullName);
