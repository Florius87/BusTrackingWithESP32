#include "ovapi.h"
#include "timing.h"
#include "display.h"
#include <WiFi.h>
#include <WiFiManager.h>

// Your WiFi credentials
const char* ssid = "YOUR_SSID";
const char* password = "WIFI_PASSWORD";

// Buffer for halte code input (max 15 chars + null terminator)
char tpc_code_buf[16] = "55000160"; // Default haltecode
WiFiManagerParameter custom_tpc_code("tpc", "Haltecode (tpc_code)", tpc_code_buf, 15);

// Use String for the rest of your code, as before
String tpc_code;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Trying hardcoded WiFi credentials...");
  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();
  const unsigned long wifiTimeout = 8000;

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifiTimeout) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected via hardcoded WiFi.");
    // Use default value from buffer if connected this way
    tpc_code = String(tpc_code_buf);
  } else {
    Serial.println("\nHardcoded WiFi failed. Starting WiFiManager...");

    WiFiManager wm;
    wm.addParameter(&custom_tpc_code);

    // Blocks here until connected and parameters filled
    bool res = wm.autoConnect("ESP32-Setup");

    if (!res) {
      Serial.println("WiFiManager failed. Restarting...");
      delay(3000);
      ESP.restart();
    }

    Serial.println("Connected via WiFiManager.");

    // Use the value from portal input (or keep default if unchanged)
    tpc_code = String(custom_tpc_code.getValue());
    Serial.print("Set haltecode to: ");
    Serial.println(tpc_code);
  }

  setupTime();
  updateOVapi();
}

void loop() {
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 60000) {
    updateOVapi();
    lastCheck = millis();
  }
}
