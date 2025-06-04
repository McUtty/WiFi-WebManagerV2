# ESP32 WifiWebManager

Ein modulares Webinterface für den ESP32 mit dynamischer Seitenverwaltung, Speicher für Einstellungen und einheitlichem, responsivem Design.

---

## Features

- **WLAN-Konfiguration** (AP/STA, Hostname, statische IP)
- **NTP-Konfiguration**
- **OTA-Firmware-Update per Web**
- **Werksreset per Web und Hardware**
- **Dynamisches Menü:** Eigene Seiten als Menüpunkt und Route aus der `.ino` hinzufügen oder entfernen
- **GET/POST-Support:** Eigene Seiten können Formulare und Aktionen unterstützen
- **Datenspeicherung:** Werte per Key/Value dauerhaft sichern
- **Automatisch einheitliches Design** für alle Seiten (Menü & Styling zentral)
- **Mobile-optimiert, modernes UI**
- **Statusausgabe per Seriell**

---

## Getting Started

### 1. Abhängigkeiten installieren

- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)
- [ArduinoOTA](https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA) (bei ESP32-Boards enthalten)

### 2. Dateien einfügen

- `WifiWebManager.h`
- `WifiWebManager.cpp`
- `main.ino` (siehe unten)

---

## Beispiel: main.ino

```cpp
#include "WifiWebManager.h"

WifiWebManager wifiWebManager;

void setup() {
    Serial.begin(115200);
    wifiWebManager.begin();

    // Beispielseite "Info" (nur GET)
    wifiWebManager.addPage(
        "Info", "/info",
        [](AsyncWebServerRequest *request) {
            return "<h1>Info</h1><p>ESP32 Webmodul: Alles läuft!</p>";
        }
    );

    // Beispielseite "Zähler" (GET und POST)
    wifiWebManager.addPage(
        "Zähler", "/counter",
        [](AsyncWebServerRequest *request) {
            String val = wifiWebManager.loadCustomData("counter", "0");
            return "<h1>Zähler</h1><form method='POST' action='/counter'>"
                   "<input name='counter' type='number' value='" + val + "'>"
                   "<input type='submit' value='Speichern'></form>"
                   "<p>Aktueller Wert: " + val + "</p>";
        },
        [](AsyncWebServerRequest *request) {
            if (request->hasParam("counter", true)) {
                wifiWebManager.saveCustomData("counter", request->getParam("counter", true)->value());
                return String("<p>Gespeichert!</p><a href='/counter'>Zurück</a>");
            }
            return String("<a href='/counter'>Zurück</a>");
        }
    );
}

void loop() {
    wifiWebManager.loop();

    // Optional: Hardware-Reset (z.B. GPIO 0 gegen GND)
    if (digitalRead(0) == LOW) {
        Serial.println("Hardware-Reset!");
        wifiWebManager.reset();
        delay(1000); // Entprellen
    }
}
```

# API-Übersicht

## API-Befehle

| Befehl                                   | Funktion                                                                                              |
|-------------------------------------------|-------------------------------------------------------------------------------------------------------|
| `begin()`                                | Startet das komplette Modul. Im `setup()` aufrufen.                                                   |
| `loop()`                                 | Muss im `loop()` laufen, erledigt alle Hintergrundaufgaben.                                           |
| `addPage(menutitel, pfad, get, post)`     | Fügt einen neuen Menüpunkt und eine Seite hinzu. GET und POST sind Handler-Funktionen (siehe unten).  |
| `removePage(pfad)`                        | Entfernt einen eigenen Menüpunkt/Seite.                                                               |
| `saveCustomData(key, wert)`               | Speichert eigene Daten persistent (Key/Value).                                                        |
| `loadCustomData(key, default)`            | Lädt eigene Daten (mit Default-Wert falls nicht gespeichert).                                         |
| `reset()`                                | Löscht alle gespeicherten Einstellungen, setzt das Gerät zurück.                                      |

Hinweis zu den Handler-Funktionen:

- Der GET-Handler erhält einen AsyncWebServerRequest* und gibt einen HTML-Body als String zurück.
- Der POST-Handler ist optional, gleiche Signatur.

Beispiel für die Handler-Nutzung:

```cpp
wifiWebManager.addPage(
    "Beispiel", "/beispiel",
    [](AsyncWebServerRequest *request) {
        return "<h1>Beispielseite</h1>";
    },
    [](AsyncWebServerRequest *request) {
        // Daten verarbeiten
        return "<p>Erfolg!</p>";
    }
);
```

Seite (Menüpunkt) hinzufügen

```cpp
wifiWebManager.addPage(
    "Mein Menü", "/meinpfad",
    [](AsyncWebServerRequest *request) {
        return "<h1>Meine Seite</h1><p>Eigener HTML-Inhalt!</p>";
    },
    // Optional: POST-Handler
    [](AsyncWebServerRequest *request) {
        // POST-Daten auswerten, speichern, etc.
        return "<p>Erfolg!</p>";
    }
);
```

Seite (Menüpunkt) entfernen

```cpp
wifiWebManager.removePage("/meinpfad");
```

Daten speichern und laden

```cpp
wifiWebManager.saveCustomData("key", "wert");
String wert = wifiWebManager.loadCustomData("key", "default");
```

Reset über Code

```cpp
wifiWebManager.reset();
```

## Standardseiten

- WLAN (Root /)
- NTP (/ntp)
- Firmware-Update (/update)
- Reset (/reset)

Eigene Seiten erscheinen automatisch im Menü (nach den Standardseiten)

# Lizenz

MIT License

Copyright (c) 2025

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

