#include <Arduino.h>
#include "serial_input.h"
//Overordnet intensjon til 
static int sisteTestverdi = 0;
static bool verdiMottatt = false;

//Variabler for Ringbuffer, x og y akse:

static const int BUFFER_STORRELSE = 100;

static int historikk[BUFFER_STORRELSE];
static int nestePlass = 0;
static int antallVerdier = 0;
static unsigned long totaltMottatt = 0;

static void lagreIBuffer(int verdi) {
    historikk[nestePlass] = verdi; //Hvor mange datapunkter motatt

    nestePlass = (nestePlass + 1) % BUFFER_STORRELSE; //Indexerer neste plass som skal skrives til

    if (antallVerdier < BUFFER_STORRELSE) { //Sjekker om vi må resete ringbuffer intex 0
        antallVerdier++;
    }

    totaltMottatt++;
}





void handleSerialInput() {
    static String tekst = "";
    static bool ugyldig = false;




    //Ingen Serial tilgjengelig, returner
    if(Serial.available() == 0) {
        return;
    }
    char tegn = Serial.read();
    Serial.print("Leste tegnkode: ");
    Serial.println((int)tegn);  

    //Oppretter at hvis man trykker enter, så skal vi sende verdien til sisteTestverdi
    if(tegn == '\n' || tegn == '\r') {
        //Ignorerer feil eller tomme linjer:
        if(tekst.length() == 0 && !ugyldig){
            return;
        }

    if (ugyldig) {
        Serial.println("Ugyldig input, send heltall");
    }
    else {
        sisteTestverdi = tekst.toInt(); //Variablen til ringbuffer og historikk
        verdiMottatt = true;
        lagreIBuffer(sisteTestverdi);

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

//RINGBUFFER: Leser fra eldste til nyeste veeerdi, selvom vi har begynt å skrive over data på tidlige indexer
String getHistorikkJson() {
    String svar;
    svar.reserve(1000);

    svar = "{\"forsteNummer\":";
    svar += String(totaltMottatt - antallVerdier + 1);
    svar += ",\"verdier\":[";

    int eldstePlass =
        (nestePlass - antallVerdier + BUFFER_STORRELSE)
        % BUFFER_STORRELSE;

    for (int i = 0; i < antallVerdier; i++) {
        int plass = (eldstePlass + i) % BUFFER_STORRELSE;

        if (i > 0) {
            svar += ",";
        }

        svar += String(historikk[plass]);
    }

    svar += "]}";
    return svar;
}