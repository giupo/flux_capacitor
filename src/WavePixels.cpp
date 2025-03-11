#include "WavePixels.hpp"
#include <cstdint>

void WavePixels::setup() {
  pixels.begin();
}

void WavePixels::update(const uint delta) {
  gap_time += delta;

  // non fare niente  meno di 300ms
  if (gap_time < GAP_TIME) return;

  gap_time = 0;

  if (is_off()) return;

//  moving_pulse(255, 125, 19, 10);
  moving_trail(230, 230, 230, 3);
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


void WavePixels::moving_pulse(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) {
    // pos = 0;
    // direction = 1; // 1 = avanti, -1 = indietro

    pixels.clear();

    constexpr static auto TAIL_SIZE = 8;
    // Crea una dissolvenza a coda
    for (int i = 0; i < TAIL_SIZE; i++) {
        int index = pos - i;
        if (index >= 0 && index < num_leds) {
            int fade = map(i, 0, TAIL_SIZE - 1, brightness, 10);
            pixels.setPixelColor(index, pixels.Color(r * fade / 255, g * fade / 255, b * fade / 255));
        }
    }

    pixels.show();

    // Movimento avanti e indietro
    pos += direction;
    if (pos >= num_leds - 1 || pos <= 0) {
        direction *= -1;
    }
}


// Effetto con dissolvenza nella scia
void WavePixels::moving_trail(uint8_t r, uint8_t g, uint8_t b, const uint8_t tail_size) {
    pixels.clear();

    // Crea la scia con effetto di dissolvenza
    for (int i = 0; i < tail_size; i++) {
        int index = pos - i;
        if (index >= 0 && index < num_leds) {
            unsigned char brightness = map(index, 0, tail_size, 255, 1); // Luminosità decrescente
            pixels.setPixelColor(
              index,
              pixels.Color(
                r, //* brightness / 255,
                g, //* brightness / 255,
                b, //* brightness / 255,
                brightness
              )
            );
        }
    }

    pixels.show();

    // Movimento avanti e indietro
    pos += direction;
    if (pos >= num_leds - 1 || pos <= 0) {
        direction *= -1;
    }
}
