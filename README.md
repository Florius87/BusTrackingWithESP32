# Live Bus Tracker with ESP32

> **A  “departure board” that shows the next buses at your stop, updated every minute via Wi-Fi.**

For a full step-by-step build guide—including wiring diagrams, photos, and advanced tips—see the accompanying blog post: 
**<https://florisera.com/live-bus-tracker-with-esp32/>**

---

## Features

- **Real-time data** pulled from the open OVapi endpoint (`https://v0.ovapi.nl/tpc/{haltecode}`) every 60 s.
- **Captive-portal config:** if your stored credentials fail, WiFiManager starts an “ESP32-Setup” AP so you can enter Wi-Fi details *and* your personal halte-code from a phone.:contentReference[oaicite:1]{index=1}  
- **Automatic NTP clock** for accurate “HH:MM” timestamp on the display.
- **20 × 4 I²C LCD** with custom bars that indicate Wi-Fi signal strength.
- **Smart formatting:** city names are abbreviated and long destinations are trimmed so everything fits on three lines.

---

## Hardware You’ll Need

| Qty | Part                                   | Notes |
|---:|----------------------------------------|-------|
| 1  | ESP32 DevKit (any USB-UART variant)    | 3 V I/O, built-in Wi-Fi |
| 1  | 20×4 I²C LCD (0x27 default address)    | Backpack already soldered |
| —  | USB-C/Micro-USB cable + 5 V supply     | For power |
| —  | Dupont wires / breadboard or enclosure | Optional tidy-up |

*(The blog post links to the exact parts I used.)*

---

## Quick Start

1. **Clone the repo** and open `ESP32Coding.ino` in the Arduino IDE / PlatformIO.  
2. **Edit** the two placeholders at the top if you want hard-coded Wi-Fi (`ssid`, `password`), or just leave them blank to rely entirely on WiFiManager.:contentReference[oaicite:5]{index=5}  
3. **Flash** the sketch to your ESP32.  
4. At first boot, join the *ESP32-Setup* hotspot, browse to `192.168.4.1`, enter your router’s credentials **and** your 8-digit halte-code.  
5. Reboot—your LCD should now show the stop name, current time, and the three soonest departures.

---

## Repository Layout
- ESP32Coding.ino        main sketch (setup & loop)
- ovapi.{h,cpp}          JSON fetch + parse helpers
- timing.{h,cpp}         NTP setup & ISO‑8601 utils
- display.{h,cpp}        LCD rendering + Wi‑Fi glyphs
- LICENSE
- README.md

---

## Customisation

- Change the **polling interval** (`60000 ms`) or the **maximum departures shown** (`3`) in `ESP32Coding.ino`.  
- Tweak city abbreviations or add new ones in `display.cpp`.  
- Swap the LCD for an OLED or e-paper display—only `display.*` needs touching.

---

## Contributing

Pull requests for bug-fixes, new display drivers, or UI tweaks are welcome. Please run `clang-format` on any C++ changes before submitting.

---

## Licence

This project is released under the **MIT License**—see `LICENSE` for details.

---

*Enjoy never missing your bus again!*

