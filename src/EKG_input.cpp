#include <Arduino.h>
#include "serial_input.h"
//Overordnet intensjon til 
static int sisteTestverdi_EKG = 0;
static bool verdiMottatt_EKG = false;

//Variabler for Ringbuffer, x og y akse:

static const int BUFFER_STORRELSE_EKG = 1000;

static int historikk[BUFFER_STORRELSE_EKG];
static int nestePlass_EKG = 0;
static int antallVerdier_EKG = 0;
static unsigned long totaltMottatt_EKG = 0;

static void lagreIBuffer_EKG(int verdi) {
    historikk[nestePlass_EKG] = verdi; //Hvor mange datapunkter motatt

    nestePlass_EKG = (nestePlass_EKG + 1) % BUFFER_STORRELSE_EKG; //Indexerer neste plass som skal skrives til

    if (antallVerdier_EKG < BUFFER_STORRELSE_EKG) { //Sjekker om vi må resete ringbuffer intex 0
        antallVerdier_EKG++;
    }

    totaltMottatt_EKG++;
}

