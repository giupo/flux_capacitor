#include "App.hpp"


#if 0


#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

const int RECV_PIN = D6;
// IRrecv irrecv(RECV_PIN);
// decode_results results;

#include <IRremote.hpp> // include the library

#define NUM_LEDS   30  // Numero di LED sulla striscia
#define SPEED      50  // Velocità dello spostamento (ms tra aggiornamenti)
#define BRIGHTNESS 255 // Luminosità massima

#define PIN_LEDS D4

ESP8266WebServer server(80);
LiquidCrystal_I2C lcd(0x3F, 20, 4); // I2C address 0x27, 20 column and 4 rows
Adafruit_NeoPixel pixels(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800);

#define IR_RECEIVE_PIN D6

bool flux_state = false;

void handleFlux() {
  JsonDocument doc;
  String body = server.arg("plain");
  deserializeJson(doc, body);
  flux_state = doc["status"];
  lcd.setCursor(0, 2);
  lcd.print("Flux ");
  lcd.print(flux_state ? "on" : "off");


  server.send(200, "text/plain", flux_state ? "on" : "off");
}

void setup() {
  Serial.begin(9600);

  Serial.println("\nScanning I2C devices...");

  Wire.begin();
  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("Dispositivo trovato all'indirizzo 0x");
      Serial.println(address, HEX);
      delay(10);
    }
  }
  Serial.println("Scansione completata.");

  lcd.begin(); //initialize the lcd
  lcd.backlight(); //open the backlight

  lcd.home();
  lcd.print("Connecting...");
  // Crea un'istanza di WiFiManager
  WiFiManager wm;

  // Avvia il captive portal se la WiFi non è configurata
  if (!wm.autoConnect("ESP_Config")) {
    lcd.home();
    lcd.print("Connessione Fallita");
    lcd.setCursor(0, 1);
    lcd.print("Riavvio...");
    delay(2000);
    ESP.restart();
  }

  // Se arriva qui, significa che è connesso alla WiFi
  lcd.home();
  lcd.print("WiFi connessa!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP().toString());
  lcd.setCursor(0, 2);

  server.on("/", HTTP_GET, []() {

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
            isOn = !isOn;
            document.querySelectorAll('.flux').forEach((el, i) => {
                if (isOn) {
                    setTimeout(() => el.classList.add('active'), i * 200);
                } else {
                    setTimeout(() => el.classList.remove('active'), i * 200);
                }
            });

            // Invia il comando alla ESP8266
            fetch('/flux', { method: 'POST', body: JSON.stringify({ status: isOn }) })
            .then(response => response.text())
            .then(data => console.log("ESP8266:", data))
            .catch(err => console.error("Errore:", err));
        }
    </script>
</body>
</html>
)rawliteral");

    server.send(200, "text/html", page);

  });

  // Avvia il server web
  server.on("/flux", HTTP_POST, handleFlux);

  server.begin();


  pixels.begin();
  pixels.setBrightness(128);
  pixels.clear();
  pixels.show();
}


void handle_ir() {
  if (!IrReceiver.decode()) return;

  flux_state = !flux_state;
  /*
   * Print a summary of received data
   */
  if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
    Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
    // We have an unknown protocol here, print extended info
    IrReceiver.printIRResultRawFormatted(&Serial, true);

    IrReceiver.resume(); // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
  } else {
    IrReceiver.resume(); // Early enable receiving of the next IR frame

    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.printIRSendUsage(&Serial);
  }
  Serial.println();

  /*
   * Finally, check the received data and perform actions according to the received command
   */
  if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
    Serial.println(F("Repeat received. Here you can repeat the same action as before."));
  } else {
    if (IrReceiver.decodedIRData.command == 0x10) {
      // do something
    } else if (IrReceiver.decodedIRData.command == 0x11) {
      // do something else
    }
  }
}

void loop_pixels() {
  if (!flux_state) {
    pixels.clear();
    pixels.show();
    return;
  }

  static float phase = 0.0;
  float waveSpeed = 0.05;  // Velocità di avanzamento dell'onda
  float frequency = 0.3;  // Densità dell'onda (più alto = più cicli sulla striscia)

  for (int i = 0; i < NUM_LEDS; i++) {
    // Calcola un valore di luminosità basato su una sinusoide
    float intensity = (sin(frequency * i + phase) + 1.0) / 2.0;
    uint8_t brightness = (uint8_t)(intensity * BRIGHTNESS);

    // Imposta il colore (es. bianco con intensità variabile)
    pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness));
  }

  pixels.show();
  phase += waveSpeed; // Avanza la fase per spostare l'onda
}

void loop() {
  server.handleClient();
  handle_ir();
  loop_pixels();
}


#endif

App app;

void setup() {
  app.setup();
}

void loop() {
  app.loop();
}
