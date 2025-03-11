#ifndef APP_H_
#define APP_H_

#include "WavePixels.hpp"
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "WebServer.hpp"

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>


#include "Observer.hpp"

struct LCDParams {
    constexpr static auto ADDRESS = 0x3F;
    constexpr static unsigned short ROWS = 20;
    constexpr static unsigned short COLS = 4;
};

struct AppParams {
    constexpr static auto AP_SSID = "ESP_Config";
    constexpr static unsigned int PIXELS_LED = D3;
    constexpr static unsigned int NUM_PIXELS = 11;
    constexpr static unsigned int HTTP_PORT = 80;
    constexpr static unsigned int UART_RX = D4;
    constexpr static unsigned int UART_TX = D5;
    constexpr static unsigned int IR_PIN = D6;
};

class App : public Observer {
    public:

        enum  IR_Keys: long long unsigned int {
          K_1 = 0xba45ff00,
          K_2 = 0xb946ff00,
          K_3 = 0xb847ff00,
          K_4 = 0xbb44ff00,
          K_5 = 0xBF40FF00,
          K_6 = 0xBC43FF00,
          K_7 = 0xF807FF00,
          K_8 = 0xEA15FF00,
          K_9 = 0xF609FF00,
          K_0 = 0xE619FF00,
          K_STAR = 0xE916FF00,
          K_POUND = 0xF20DFF00,
          K_UP = 0xE718FF00,
          K_DOWN = 0xAD52FF00,
          K_LEFT = 0xF708FF00,
          K_RIGHT = 0xA55AFF00,
          K_OK = 0xE31CFF00
        };

        App():
            pixels(AppParams::PIXELS_LED, AppParams::NUM_PIXELS),
            lcd(LCDParams::ADDRESS,  LCDParams::ROWS, LCDParams::COLS),
            server(AppParams::HTTP_PORT),
            serial(AppParams::UART_RX, AppParams::UART_TX)
        {
            start_time = last_time = now = 0;
        }

        virtual ~App() = default;

        void setup();
        void setup_lcd();
        void setup_audio();
        void setup_wifi();
        void setup_ir();

        virtual void update() override {}
        void update(const unsigned int delta);
        void loop();

    private:
        unsigned int start_time;
        unsigned int last_time;
        unsigned int now;

        WavePixels pixels;
        LiquidCrystal_I2C lcd;// I2C address 0x3F, 20 column and 4 rows
        WebServer server;

        SoftwareSerial serial; // (14, 12); // RX su GPIO14 (D5), TX su GPIO12 (D6)
        DFRobotDFPlayerMini player;
};


#endif // APP_H_
