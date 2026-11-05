#include <Arduino.h>

// Egne headerfiler
#include "wifi_ap.h"
#include "web_server.h"
#include "serial_input.h"

//test push git

void setup() {
    Serial.begin(115200);
    Serial.println("Initial boot success");

    bool nettverkStartet = startWiFi();

    if (nettverkStartet) {
        startWebServer();
    }
}

//testendring


void loop() {
    handleSerialInput(); //Sender dataen som kommmer inn fra seriell
    handleWebClient(); //Stuker m webserveren. Black magic
    delay(2);
}