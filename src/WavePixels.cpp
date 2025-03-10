#include "WavePixels.hpp"

void WavePixels::setup() {
  pixels.begin();
}

void WavePixels::update(const uint delta) {
  clear();
  for (int i = 0; i < num_leds; i++) {
    // Calcola un valore di luminosità basato su una sinusoide
    // float intensity = (sin(frequency * i + phase) + 1.0) / 2.0;
    // uint8_t brightness = (uint8_t)(intensity * BRIGHTNESS);
    uint8_t brightness = wave[i] * BRIGHTNESS;

    // Imposta il colore (es. bianco con intensità variabile)
    pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness));
  }
}

void WavePixels::render() {
  if (is_off()) clear();
  pixels.show();
}

void WavePixels::clear() {
  pixels.clear();
}

void WavePixels::turn_on() {
  state = true;
  Serial.println("Pixels are On!");
}
void WavePixels::turn_off() {
  state = false;
  Serial.println("Pixels are Off!");
}
