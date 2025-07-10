#include <Arduino.h>
#include <time.h>

void setupTime() {
  configTime(3600, 3600, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  Serial.print("Waiting for NTP time sync");
  while (now < 100000) {
    delay(100);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println(" done!");
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  Serial.printf("Current time: %02d:%02d:%02d %02d-%02d-%04d\n",
    timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec,
    timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
}

time_t parseISO8601(const String& iso) {
  struct tm t;
  memset(&t, 0, sizeof(t));
  t.tm_year = iso.substring(0, 4).toInt() - 1900;
  t.tm_mon  = iso.substring(5, 7).toInt() - 1;
  t.tm_mday = iso.substring(8, 10).toInt();
  t.tm_hour = iso.substring(11, 13).toInt();
  t.tm_min  = iso.substring(14, 16).toInt();
  t.tm_sec  = iso.substring(17, 19).toInt();
  t.tm_isdst = -1;
  return mktime(&t);
}
