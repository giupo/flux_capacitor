#include "WavePixels.hpp"


void WavePixels::update(const uint delta) {
  if (!state) return;

  static float phase = 0.0;
  float waveSpeed = 0.05;  // Velocità di avanzamento dell'onda
  float frequency = 0.3;  // Densità dell'onda (più alto = più cicli sulla striscia)

  for (int i = 0; i < num_leds; i++) {
    // Calcola un valore di luminosità basato su una sinusoide
    // float intensity = (sin(frequency * i + phase) + 1.0) / 2.0;
    // uint8_t brightness = (uint8_t)(intensity * BRIGHTNESS);
    uint8_t brightness = wave[i] * BRIGHTNESS;

    // Imposta il colore (es. bianco con intensità variabile)
    pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness));
  }

  phase += waveSpeed; // Avanza la fase per spostare l'onda

}

void WavePixels::render() {
    pixels.show();
}

void WavePixels::clear() {
    pixels.clear();
}
