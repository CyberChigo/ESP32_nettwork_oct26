#include <Arduino.h>
#include "serial_input.h"

static int sisteTestverdi = 0;
static bool verdiMottatt = false;

void handleSerialInput() {
    static String tekst = "";
    static bool ugyldig = false;


    //Ingen Serial tilgjengelig, returner
    if(Serial.available() == 0) {
        return;
    }
    char tegn = Serial.read();  

    //Oppretter at hvis man trykker enter, så skal vi sende verdien til sisteTestverdi
    if(tegn == '\n' || tegn == '\r') {
        //Ignorerer feil eller tomme linjer:
        if(tekst.length() == 0 && !ugyldig){
            return;
        }

        if(ugyldig){
            Serial.println("Ugyldig input, send heltall");
        }
        else{
            sisteTestverdi = tekst.toInt(); //omgjør input til integer for å kunne plotte
            verdiMottatt = true;
            Serial.println("Verdi motatt: ");
            Serial.println(sisteTestverdi);
        }

        //soft reset av tallvariabel
        tekst = "";
        ugyldig = false;
    }
    else{
        if (tegn >='0' && tegn <= '9' && tekst.length()<4){
            tekst += tegn;
        }
        
        else {
            ugyldig = true; //Helgardering :D
        }
    }
}

int getTestverdi() {
    return sisteTestverdi;
}

bool harTestverdi() {
    return verdiMottatt;
}