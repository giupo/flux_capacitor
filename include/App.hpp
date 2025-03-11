#ifndef APP_H_
#define APP_H_

#include "WavePixels.hpp"
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "WebServer.hpp"

struct LCDParams {
    constexpr static auto ADDRESS = 0x3F;
    constexpr static unsigned short ROWS = 20;
    constexpr static unsigned short COLS = 4;
};

struct AppParams {
    constexpr static auto AP_SSID = "ESP_Config";
    constexpr static unsigned int PIXELS_LED = D4;
    constexpr static unsigned int NUM_PIXELS = 11;
    constexpr static unsigned int HTTP_PORT = 80;
};

class App {
    public:
        App():
            pixels(AppParams::PIXELS_LED, AppParams::NUM_PIXELS),
            lcd(LCDParams::ADDRESS,  LCDParams::ROWS, LCDParams::COLS),
            server(AppParams::HTTP_PORT)
        {
            start_time = last_time = now = 0;
        }

        virtual ~App() = default;

        void setup();
        void setup_lcd();
        void setup_wifi();

        void update(const unsigned int delta);
        void loop();

    private:
        unsigned int start_time;
        unsigned int last_time;
        unsigned int now;

        WavePixels pixels;
        LiquidCrystal_I2C lcd;// I2C address 0x3F, 20 column and 4 rows
        WebServer server;
};


#endif // APP_H_
