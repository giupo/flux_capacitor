#include "App.hpp"

#include <ESP8266WiFi.h>
#include <WiFiManager.h>

void App::setup_lcd() {
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
}

void App::setup() {
    start_time = last_time = millis();
    setup_lcd();
    setup_wifi();
    server.setup();
}

void App::update(const unsigned int delta) {
    server.loop();
    pixels.update(delta);
}

void App::loop() {
    now = millis();
    const int delta = now-last_time;
    update(delta);
    pixels.render();
    last_time = now;
}
