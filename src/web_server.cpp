#include <Arduino.h>
#include <WebServer.h>
#include "web_server.h"

static WebServer server(80);


// Håndterer forespørsler til Hovedsiden samt byggere basic HTML kode for videre sending :D
void handleRoot_1() {
    Serial.println("Foresporsel mottatt for /");

    server.send(200, "text/html; charset=utf-8", R"HTML(
<!DOCTYPE html>
<html lang="no">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>EKG – Hovedside</title>

    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            padding: 40px 16px;
            background-color: #e8e9eb;
        }

        .PULS_knapp {
            display: inline-block;
            padding: 16px 32px;
            margin: 10px;
            background-color: #1456b8;
            color: white;
            text-decoration: none;
            border-radius: 10px;
        }

        .HRV_knapp {
            display: inline-block;
            padding: 16px 32px;
            margin: 10px;
            background-color: #800561;
            color: white;
            text-decoration: none;
            border-radius: 10px;
        }

        .knapp:hover {
            background-color: #555354;
        }

        .knapp:active {
            transform: scale(0.96);
        }
    </style>
</head>


<body>
    <h1>Velkommen til EKG-Life support system </h1>
    <h4>(Skal kun benyttes av uprofesjonelle)</h4>
    <p>Hvilken side vil du til? :</p>

<!-- Linker til Puls-siden --> 
    <a class="PULS_knapp" href="/puls">Puls</a> 


<!-- Linker til HRV-siden -->
    <a class="HRV_knapp" href="/hrv">HRV</a>
</body>
</html>
)HTML");
}



// Håndterer forespørsler til /PULS
void handleRoot_2() {
    Serial.println("Foresporsel mottatt for /PULS");

    server.send(
        200,
        "text/html",
        "<h1>ESP32 fungerer for Pulssiden! </h1>"
    );
}

// Håndterer forespørsler til /HRV
void handleRoot_3() {
    Serial.println("Foresporsel mottatt for /HRV");

    server.send(
        200,
        "text/html",
        "<h1>ESP32 fungerer for HRV-siden!</h1>"
    );
}

//Starer Serveren og fikser URLfor subseksjonene


void startWebServer() {
    // Registrer alle URL-rutene
    server.on("/", HTTP_GET, handleRoot_1);

    server.on("/PULS", HTTP_GET, handleRoot_2);
    server.on("/puls", HTTP_GET, handleRoot_2);
    server.on("/Puls", HTTP_GET, handleRoot_2);

    server.on("/HRV", HTTP_GET, handleRoot_3);
    server.on("/hrv", HTTP_GET, handleRoot_3);
    server.on("/Hrv", HTTP_GET, handleRoot_3);

    // Svar når ingen registrert rute passer
    server.onNotFound([]() {
        Serial.print("Fant ingen rute for: ");
        Serial.println(server.uri());

        server.send(
            404,
            "text/plain",
            String("Fant ingen rute for: ") + server.uri()
        );
    });

    server.begin();
    Serial.println("HTTP-serveren er oppe!");
}


//Håndterer datta fra og til ESP32en

void handleWebClient() {
    server.handleClient();
}