#pragma once
#include <Arduino.h>
#include <time.h>

void setupTime();
time_t parseISO8601(const String& iso);
unsigned long parseTime(const String& iso);