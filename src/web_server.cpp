#include <Arduino.h>
#include <WebServer.h>
#include "web_server.h"
#include "serial_input.h"

static WebServer server(80);


// Håndterer forespørsler til Hovedsiden samt bygger basic HTML kode for videre sending :D
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

        .Playground_knapp {
            display: inline-block;
            padding: 16px 32px;
            margin: 10px;
            background-color: #0e244a;
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

<!-- HOVEDSIDE --> 

<body>
    <h1>Velkommen til EKG-Life support system </h1>
    <h4>(Skal kun benyttes av uprofesjonelle)</h4>
    <p>Hvilken side vil du til? :</p>

<!-- Linker til Puls-siden --> 
    <a class="PULS_knapp" href="/puls">Puls</a> 


<!-- Linker til HRV-siden -->
    <a class="HRV_knapp" href="/hrv">HRV</a>

<!-- Linker til Playground-siden -->
    <a class="Playground_knapp" href="/playground">Playground</a>

</body>
</html>
)HTML");
}



// Håndterer forespørsler til /PULS
void handleRoot_2() {
    Serial.println("Foresporsel mottatt for /PULS");

    server.send(
        200,
        "text/html; charset=utf-8", //Lar æ - bruke norske bokstaver
            R"HTML( 
            <!DOCTYPE html>
            <html lang="no">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <title>PULS</title>
        </head>

        <body>
            <h1>PULS</h1>
            <p>PULS siden fungerer! :D </p>
            <a href="/">Tilbake til hovedsiden</a>
        </body>
        
        </html>
        )HTML"
    );
}



// Håndterer forespørsler til /HRV
void handleRoot_3() {
    Serial.println("Foresporsel mottatt for /HRV");

    server.send(
        200,
        "text/html; charset=utf-8", //Lar æ - bruke norske bokstaver
            R"HTML( 
            <!DOCTYPE html>
            <html lang="no">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <title>HRV</title>
        </head>

        <body>
            <h1>HRV</h1>
            <p>HRV siden fungerer! :D </p>
            <a href="/">Tilbake til hovedsiden</a>
        </body>
        
        </html>
        )HTML"
        
    );
}



// Håndterer forespørsler til /Playground
// Inkluderer alle visuelle inpus, HTML blir fooort stor.


void handleRoot_4() {
    server.send(200, "text/html; charset=utf-8", R"HTML(
<!DOCTYPE html>
<html lang="no">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Playground</title>

    <style>
        body {
            font-family: Arial, sans-serif;
            max-width: 900px;
            margin: auto;
            padding: 20px;
            background: #e8e9eb;
        }

        canvas {
            width: 100%;
            height: auto;
            background: white;
            border: 1px solid #aaa;
        }
    </style>
</head>

<body>
    <h1>Playground</h1>
    <p>Siste testverdi: <strong id="siste">—</strong></p>
    <p id="status">Henter historikk …</p>

    <canvas id="graf" width="900" height="400"
            aria-label="Graf over sendte testverdier">
        Nettleseren støtter ikke Canvas.
    </canvas>

    <p>X-akse: målenummer. Y-akse: testverdi.</p>
    <a href="/">Tilbake til hovedsiden</a>

    <script>
        const canvas = document.getElementById("graf");
        const ctx = canvas.getContext("2d");
        const statusfelt = document.getElementById("status");
        const sistefelt = document.getElementById("siste");

        function tegnGraf(data) {
            const verdier = data.verdier;

            const venstre = 70;
            const topp = 25;
            const bredde = canvas.width - venstre - 30;
            const hoyde = canvas.height - topp - 55;

            ctx.clearRect(0, 0, canvas.width, canvas.height);
            ctx.font = "14px Arial";

            if (verdier.length === 0) {
                ctx.fillStyle = "#333";
                ctx.fillText("Send et tall fra seriellmonitoren.", 70, 80);
                sistefelt.textContent = "—";
                return;
            }

            sistefelt.textContent = verdier[verdier.length - 1];

            // Y-aksen starter på null og tilpasses høyeste verdi.
            const maks = Math.max(10, ...verdier) * 1.1;

            // Tegn rutenett og tall på Y-aksen.
            for (let i = 0; i <= 5; i++) {
                const y = topp + hoyde - (i / 5) * hoyde;
                const verdi = (i / 5) * maks;

                ctx.strokeStyle = "#ddd";
                ctx.beginPath();
                ctx.moveTo(venstre, y);
                ctx.lineTo(venstre + bredde, y);
                ctx.stroke();

                ctx.fillStyle = "#333";
                ctx.textAlign = "right";
                ctx.fillText(verdi.toFixed(0), venstre - 10, y + 5);
            }

            // Gjør måleverdier om til posisjoner på tegneflaten.
            const punkter = verdier.map((verdi, i) => ({
                x: venstre + i / Math.max(1, verdier.length - 1) * bredde,
                y: topp + hoyde - verdi / maks * hoyde
            }));

            // Tegn linjen mellom punktene.
            ctx.strokeStyle = "#1456b8";
            ctx.lineWidth = 2;
            ctx.beginPath();

            punkter.forEach((punkt, i) => {
                if (i === 0) {
                    ctx.moveTo(punkt.x, punkt.y);
                } else {
                    ctx.lineTo(punkt.x, punkt.y);
                }
            });

            ctx.stroke();

            // Tegn punktene, også når vi bare har én verdi.
            ctx.fillStyle = "#1456b8";

            punkter.forEach(punkt => {
                ctx.beginPath();
                ctx.arc(punkt.x, punkt.y, 3, 0, Math.PI * 2);
                ctx.fill();
            });

            // Vis første og siste målenummer.
            ctx.fillStyle = "#333";
            ctx.textAlign = "left";
            ctx.fillText(
                String(data.forsteNummer),
                venstre,
                topp + hoyde + 25
            );

            if (verdier.length > 1) {
                ctx.textAlign = "right";
                ctx.fillText(
                    String(data.forsteNummer + verdier.length - 1),
                    venstre + bredde,
                    topp + hoyde + 25
                );
            }
        }

        async function oppdaterGraf() {
            try {
                const svar = await fetch("/api/historikk", {
                    cache: "no-store"
                });

                if (!svar.ok) {
                    throw new Error("Kunne ikke hente historikk");
                }

                const data = await svar.json();

                tegnGraf(data);
                statusfelt.textContent =
                    data.verdier.length + " av 100 bufferplasser i bruk.";
            }
            catch (feil) {
                statusfelt.textContent =
                    "Ingen kontakt. Grafen viser sist hentede historikk.";
            }

            setTimeout(oppdaterGraf, 500);
        }

        oppdaterGraf();
    </script>
</body>
</html>
)HTML");
}




void handleRoot_5() {
    Serial.println("Foresporsel mottatt for /EKG");

    server.send(
        200,
        "text/html; charset=utf-8", //Lar nettsiden benytte seg av norske bokstaver
            R"HTML( 
            <!DOCTYPE html>
            <html lang="no">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <title>EKG monitor</title>
        </head>

        <body>
            <h1>EKG</h1>
            <p>EKG siden fungerer! :D </p>
            <a href="/">Tilbake til hovedsiden</a>
        </body>
        
        </html>
        )HTML"
        
    );
}


void handleTestverdi() {
    String svar;

    if (harTestverdi()) {
        svar = String("{\"verdi\":") + getTestverdi() + "}";
    }
    else {
        svar = "{\"verdi\":null}";
    }

    server.sendHeader("Cache-Control", "no-store");
    server.send(200, "application/json", svar);
}










//

void handleHistorikk() {
    server.sendHeader("Cache-Control", "no-store");

    server.send(
        200,
        "application/json",
        getHistorikkJson()
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

    server.on("/Plaground", HTTP_GET, handleRoot_4);
    server.on("/PLAYGROUND", HTTP_GET, handleRoot_4);
    server.on("/playground", HTTP_GET, handleRoot_4);

    server.on("/EKG", HTTP_GET, handleRoot_5);
    server.on("/ekg", HTTP_GET, handleRoot_5);
    server.on("/Ekg", HTTP_GET, handleRoot_5);

    server.on("/api/testverdi", HTTP_GET, handleTestverdi); //For å printe til playground
    server.on("/api/historikk", HTTP_GET, handleHistorikk); //For å printe historikk i graf playground

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