#include <Arduino.h>

// Egne headerfiler
#include "wifi_ap.h"
#include "web_server.h"

void setup() {
    Serial.begin(115200);
    Serial.println("Initial boot success");

    bool nettverkStartet = startWiFi();

    if (nettverkStartet) {
        startWebServer();
    }
}

void loop() {
    handleWebClient();
    delay(2);
}