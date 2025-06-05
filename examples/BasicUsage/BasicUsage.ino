#include <WiFiWebManager.h>


WifiWebManager wifiWebManager;

void setup() {
    Serial.begin(115200);
    wifiWebManager.begin();

    // Info-Seite (GET-only)
    wifiWebManager.addPage(
        "Info", "/info",
        [](AsyncWebServerRequest *request) {
            return "<h1>Info</h1><p>ESP32 Webmodul: Alles läuft!</p>";
        }
    );

    // Zähler-Seite (GET/POST)
    wifiWebManager.addPage(
        "Zähler", "/counter",
        // GET-Handler
        [](AsyncWebServerRequest *request) {
            String val = wifiWebManager.loadCustomData("counter", "0");
            return "<h1>Zähler</h1><form method='POST' action='/counter'>"
                   "<input name='counter' type='number' value='" + val + "'>"
                   "<input type='submit' value='Speichern'></form>"
                   "<p>Aktueller Wert: " + val + "</p>";
        },
        // POST-Handler
        [](AsyncWebServerRequest *request) {
            String info = "";
            if (request->hasParam("counter", true)) {
                wifiWebManager.saveCustomData("counter", request->getParam("counter", true)->value());
                info = "<p>Gespeichert!</p>";
            }
            return info + "<a href='/counter'>Zurück</a>";
        }
    );
}

void loop() {
    wifiWebManager.loop();

    // Hardware-Reset Beispiel (GPIO 0 auf GND)
    // if (digitalRead(0) == LOW) {
    //     Serial.println("Hardware-Reset!");
    //     wifiWebManager.reset();
    //     delay(1000); // entprellen
    // }
}
