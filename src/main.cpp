#include "App.hpp"

App app;

void setup() {
  Serial.begin(115200);
  app.setup();
}

void loop() {
  app.loop();
}
