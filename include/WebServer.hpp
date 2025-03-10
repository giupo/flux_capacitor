#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include <ESP8266WebServer.h>


// forward declarations:
class WavePixels;
class LiquidCrystal_I2C;

class WebServer {
    public:
        WebServer(const unsigned int port): server(port) {}
        virtual ~WebServer() = default;

        void setup();
        void loop();

        // handlers
        void handle_flux();

        WavePixels* pixels = nullptr;
        LiquidCrystal_I2C* lcd = nullptr;
    private:
        ESP8266WebServer server;
};

#endif // WEBSERVER_H_
