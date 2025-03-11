#include "App.hpp"

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <IRremote.hpp> // include the library


void App::setup_lcd() {
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
}

void App::setup_wifi() {
    WiFiManager wm;

    lcd.home();
    lcd.print("Connecting..");

    // Avvia il captive portal se la WiFi non è configurata
    if (!wm.autoConnect(AppParams::AP_SSID)) {
        lcd.home();
        lcd.print("Connessione Fallita");
        lcd.setCursor(0, 1);
        lcd.print("Riavvio...");
        delay(2000);
        ESP.restart();
    }

    // Se arriva qui, significa che è connesso alla WiFi
    lcd.home();
    lcd.print("IP: ");
    lcd.setCursor(4, 0);
    lcd.print(WiFi.localIP().toString());
    lcd.setCursor(0, 1);
}

void App::setup_audio() {
    serial.begin(9600); // Comunicazione con il modulo
    serial.print("ciao mondo");
    if (!player.begin(serial)) {
        Serial.println("Modulo audio non trovato!");
        return;
    }

    Serial.println("Modulo audio pronto");
    player.volume(20); // Imposta il volume (0-30)
    player.play(1); // Riproduce il primo file nella SD
}

void App::setup_ir() {
    IrReceiver.begin(AppParams::IR_PIN, ENABLE_LED_FEEDBACK);
    Serial.println("Ricevitore IR pronto!");
}

void App::setup() {
    start_time = last_time = millis();

    // turn off lights as soon as possibile
    pixels.setup();

    setup_lcd();
    setup_wifi();
    setup_audio();
    setup_ir();

    server.lcd = &lcd;
    server.pixels = &pixels;

    server.setup();
    pixels.turn_off();
}

void App::update(const unsigned int delta) {
    server.loop();
    if (IrReceiver.decode()) {  // Controlla se è stato ricevuto un segnale
        Serial.print("Codice ricevuto: ");
        IR_Keys pressed = (IR_Keys) IrReceiver.decodedIRData.decodedRawData;
        Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); // Mostra il codice IR ricevuto in HEX
        IrReceiver.resume();
        if (pressed == IR_Keys::K_OK) {
            pixels.turn_on();
        }

    }
    pixels.update(delta);
}

void App::loop() {
    now = millis();
    const int delta = now - last_time;
    update(delta);
    pixels.render();
    last_time = now;
}
