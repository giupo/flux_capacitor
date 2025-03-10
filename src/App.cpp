#include "App.hpp"

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

void App::setup_lcd() {
    lcd.begin(); //initialize the lcd
    lcd.backlight(); //open the backlight
}

void App::setup_wifi() {
    WiFiManager wm;

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
    lcd.print("WiFi connessa!");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP().toString());
    lcd.setCursor(0, 2);
}

void App::setup() {
    start_time = last_time = millis();
    setup_lcd();
    setup_wifi();

    pixels.setup();
    server.lcd = &lcd;
    server.pixels = &pixels;

    server.setup();
    pixels.turn_off();
}

void App::update(const unsigned int delta) {
    server.loop();
    pixels.update(delta);
}

void App::loop() {
    now = millis();
    const int delta = now - last_time;
    update(delta);
    pixels.render();
    last_time = now;
}
