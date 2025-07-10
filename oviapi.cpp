#include "ovapi.h"
#include "timing.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "display.h"

bool fetchOVapiPayload(const String& halteCode, String& payloadOut) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi not connected.");
    return false;
  }

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  String url = "https://v0.ovapi.nl/tpc/" + halteCode;
  if (!http.begin(client, url)) {
    Serial.println("HTTP begin() failed.");
    return false;
  }

  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("HTTP error: %d\n", httpCode);
    http.end();
    return false;
  }

  payloadOut = http.getString();
  http.end();
  return true;
}

void updateOVapi() {
  extern String tpc_code;

  String payload;
  if (!fetchOVapiPayload(tpc_code, payload)) {
    Serial.println("Error fetching OVapi data.");
    return;
  }

  DynamicJsonDocument doc(8192);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("JSON parse failed: ");
    Serial.println(error.c_str());
    return;
  }

  JsonObject halte = doc[tpc_code];
  JsonObject passes = halte["Passes"];

  Departure departures[50];
  int depCount = 0;

  time_t now = time(nullptr);

  for (JsonPair kv : passes) {
    JsonObject p = kv.value().as<JsonObject>();
    const char* lijn = p["LinePublicNumber"];
    const char* bestemming = p["DestinationName50"];
    const char* tijd = p["ExpectedDepartureTime"];
    if (!tijd) tijd = p["ExpectedArrivalTime"];
    if (!(lijn && bestemming && tijd)) continue;

    time_t arrival = parseISO8601(tijd);
    long minLeft = (arrival - now) / 60;

    departures[depCount].line = lijn;
    departures[depCount].destination = bestemming;
    departures[depCount].time = tijd;
    departures[depCount].minLeft = minLeft;
    depCount++;
    if (depCount >= 50) break;
  }

  // Sort by soonest
  for (int i = 0; i < depCount - 1; i++) {
    for (int j = i + 1; j < depCount; j++) {
      if (parseISO8601(departures[j].time) < parseISO8601(departures[i].time)) {
        Departure tmp = departures[i];
        departures[i] = departures[j];
        departures[j] = tmp;
      }
    }
  }

  String halteNaam = "";
  if (halte.containsKey("Stop")) {
    JsonObject stopinfo = halte["Stop"];
    if (stopinfo.containsKey("TimingPointName")) {
      halteNaam = stopinfo["TimingPointName"].as<const char*>();
    }
  }


  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  char tijdStr[6];
  snprintf(tijdStr, sizeof(tijdStr), "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
  printLCD(halteNaam, tijdStr, departures, depCount);
}
