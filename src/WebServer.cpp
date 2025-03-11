#include "WebServer.hpp"

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

#include "WavePixels.hpp"

void WebServer::setup() {

  server.on("/", HTTP_GET, [this]() {
    String page = (R"rawliteral(
<!DOCTYPE html>
<html lang="it">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Flusso Canalizzatore</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: black;
            color: white;
            text-align: center;
            padding: 20px;
        }
        .flux-container {
            display: flex;
            justify-content: center;
            gap: 10px;
            margin-top: 30px;
        }
        .flux {
            width: 20px;
            height: 60px;
            background-color: grey;
            border-radius: 5px;
            transition: background 0.2s;
        }
        .active {
            background: red;
            box-shadow: 0px 0px 10px red;
        }
        button {
            margin-top: 20px;
            padding: 10px 20px;
            font-size: 18px;
            background: #ff0000;
            color: white;
            border: none;
            cursor: pointer;
            border-radius: 5px;
        }
        button:hover {
            background: #cc0000;
        }
    </style>
</head>
<body>
    <h1>FLUSSO CANALIZZATORE</h1>
    <h2>Capace di generare 1.21 Gigawatt!</h2>

    <div class="flux-container">
        <div class="flux" id="flux1"></div>
        <div class="flux" id="flux2"></div>
        <div class="flux" id="flux3"></div>
    </div>

    <button onclick="toggleFlux()">Attiva/Disattiva</button>

    <script>
        let isOn = false;
        function toggleFlux() {

            fetch('/flux', { method: 'GET'})
              .then(response => response.text())
              .then((data) =>  {
                 let isOn = false;
                 if (data === "on") {
                   isOn = true;
                 }

                 const gap = 200; // m-seconds
                 const duration = 3000; // m-seconds
                 // flip
                 isOn = !isOn;
                 document.querySelectorAll('.flux').forEach((el, i) => {
                   if (isOn) {
                      setTimeout(() => el.classList.add('active'), i * gap);
                      setTimeout(() => el.classList.remove('active'), (i * gap) + duration);

                   } else {
                      setTimeout(() => el.classList.remove('active'), i * gap);
                   }
                 });

                 fetch('/flux', { method: 'POST', body: JSON.stringify({ status: isOn }) })
                   .then(response => response.text())
                   .then(data => console.log("Flux:", data))
                   .catch(err => console.error("Errore:", err));
            });

        }
    </script>
</body>
</html>
)rawliteral");

    server.send(200, "text/html", page);

  });


  // Avvia il server web
  server.on("/flux", HTTP_POST, std::bind(&WebServer::handle_flux, this));
  server.on("/flux", HTTP_GET, [this]() {
    server.send(200, "text/plain", pixels->is_on() ? "on" : "off");
  });

  server.on("/reset", HTTP_GET, std::bind(&WebServer::handle_reset, this));

  server.begin();
}

void WebServer::loop() {
    server.handleClient();
}

void WebServer::handle_flux() {
  JsonDocument doc;
  String body = server.arg("plain");
  deserializeJson(doc, body);
  bool command_state = doc["status"];

  if (!lcd || !pixels) {
    // no lcd or pixels? Nothing to do here..
    server.send(200, "text/plain", "N/A");
    return;
  }

  lcd->setCursor(0, 1);
  lcd->print("Flux ");
  command_state ? pixels->turn_on() : pixels->turn_off();
  lcd->print(pixels->is_on() ? "on " : "off");
  server.send(200, "text/plain", pixels->is_on() ? "on" : "off");
}

void WebServer::handle_reset() {
  if (lcd) {
    lcd->home();
    lcd->print("Richiesto Reset");
    lcd->setCursor(0, 1);
    lcd->print("Riavvio...");
  }
  delay(2000);
  WiFi.disconnect();
  ESP.restart();
}
