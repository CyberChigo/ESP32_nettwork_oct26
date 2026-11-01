//For å kunne skrive random tall til serial monitor og grafe dette
#pragma once
#include<Arduino.h>


void handleSerialInput();
int getTestverdi();
bool harTestverdi();

String getHistorikkJson();