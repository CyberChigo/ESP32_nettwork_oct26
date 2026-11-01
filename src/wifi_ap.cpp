#include <WiFi.h>
#include <Arduino.h>

#include "wifi_ap.h"  //Egen headerfil under "include"


bool startWiFi(){
    WiFi.mode(WIFI_AP);

    bool nettverkStartet = WiFi.softAP( //Oppretter ESPens egen nettverk m navn og passord
        "ESP32_AP_EKG",
        "Password123"
    );

    if (nettverkStartet){
        Serial.println("Wifi er oppe! Yippiy");
        Serial.println("IP-adressen er: ");
        Serial.println(WiFi.softAPIP());//Henter IP og priner il seriell
    }
    else{
        Serial.println("Klarte ikke opprettet WiFi.. ");
    }
    return nettverkStartet;
}