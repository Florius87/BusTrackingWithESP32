#include "display.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>


byte wifi1[8] = {  // 1 bar
  B00000,
  B00000,
  B00000,
  B00000,
  B10000,
  B10000,
  B10000,
  B00000
};
byte wifi2[8] = {  // 2 bars
  B00000,
  B00000,
  B00100,
  B00100,
  B10100,
  B10100,
  B10100,
  B00000
};
byte wifi3[8] = {  // 3 bars
  B00001,
  B00001,
  B00101,
  B00101,
  B10101,
  B10101,
  B10101,
  B00000
};


LiquidCrystal_I2C lcd(0x27, 20, 4);

String abbreviateCity(const String& city) {
  if (city == "IJmuiden") return "IJm";
  if (city == "Haarlem") return "Hlm";
  if (city == "Amsterdam") return "Ams";
  if (city == "Santpoort-Noord") return "SptN";
  if (city == "Beverwijk") return "Bvw";
  if (city == "Heemstede") return "Hsd";
  if (city == "Bloemendaal") return "Bld";
  if (city == "Driehuis") return "Drh";
  return city.substring(0, 3);
}

String shortenHalteName(const String& fullName) {
  int commaIndex = fullName.indexOf(',');
  if (commaIndex == -1) return fullName.substring(0, 14);

  String city = fullName.substring(0, commaIndex);
  String street = fullName.substring(commaIndex + 1);
  street.trim();
  String shortCity = abbreviateCity(city);

  String result = shortCity + ", " + street;
  if (result.length() > 14)
    result = result.substring(0, 14);
  return result;
}


void printLCD(const String& halteNaam, const char* tijdStr, Departure departures[], int depCount) {
  static bool lcdStarted = false;
  if (!lcdStarted) {
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, wifi1); // Register custom char at index 0
    lcd.createChar(1, wifi2); // Index 1
    lcd.createChar(2, wifi3); // Index 2
    lcdStarted = true;
  }
  lcd.clear();

  int32_t rssi = WiFi.RSSI();
  int wifiChar = 0;
  if      (rssi > -60) wifiChar = 2;      // Strongest (3 bars)
  else if (rssi > -75) wifiChar = 1;      // Medium (2 bars)
  else                 wifiChar = 0;      // Weak (1 bar)

  // Row 0: Halte (max 13 chars) + time (right)
  String row0 = shortenHalteName(halteNaam);
  if (row0.length() > 13) row0 = row0.substring(0, 13);
  lcd.setCursor(0, 0);
  lcd.print(row0);
  lcd.setCursor(14, 0);
  lcd.print(tijdStr);
  lcd.setCursor(19, 0);      // right after the time string
  lcd.write(wifiChar);       // Prints the chosen WiFi symbol

  // Rows 1–3: up to 3 busses
  int shown = 0;
  for (int i = 0; i < depCount && shown < 3; i++) {
    if (departures[i].minLeft < 0) continue;  // Skip departed buses

    lcd.setCursor(0, shown + 1);
    String lijn = departures[i].line;
    String dest = departures[i].destination;
    if (dest.length() > 11) dest = dest.substring(0, 11);
    long minLeft = departures[i].minLeft;
    char minleft[5];
    snprintf(minleft, 5, "%2ldm", minLeft);

    char row[21];
    snprintf(row, 21, "%-3s %-10s %4s", lijn.c_str(), dest.c_str(), minleft);
    lcd.print(row);

  shown++;
  }

}
