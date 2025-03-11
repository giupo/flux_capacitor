#include "WavePixels.hpp"
#include <cstdint>

void WavePixels::setup() {
  pixels.begin();
}

void WavePixels::update(const uint delta) {
  // record time passing by...
  gap_time += delta;

  // non fare niente
  if (gap_time < assigned_gap_time) return;
  gap_time = 0;

  if (state == State::PULSE) {
    moving_pulse(255, 220, 190, 5);

    assigned_gap_time--;
    if (assigned_gap_time == 0) {
      flash();
    }
  } else if (state == State::FLASH) {
    moving_flash();

    if (count_flash == 0) {
      turn_off();
    }
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
  pulse();
}

void WavePixels::turn_off() {
  pos = 0;
  state = State::OFF;
  Serial.println("Pixels are Off!");
}


void WavePixels::pulse() {
  assigned_gap_time = 80;
  pos = 0;
  gap_time = 0;
  state = State::PULSE;
  Serial.println("Pixels are pulsating!");
}

void WavePixels::flash() {
  assigned_gap_time = 100;
  count_flash = 15;
  pos = 0;
  gap_time = 0;
  state = State::FLASH;
  Serial.println("Pixels are flashing!");
}

// Effetto con dissolvenza nella scia
void WavePixels::moving_pulse(
  const uint8_t r,
  const uint8_t g,
  const uint8_t b,
  const uint8_t tail_size
) {
  pixels.clear();

  // Crea la scia con effetto di dissolvenza
  for (int i = 0; i < tail_size; i++) {
    int index = pos - i;
    if (index >= 0 && index < num_leds) {
      unsigned char brightness = map(index, 0, tail_size, MAX_VALUE, 20); // Luminosità decrescente
      pixels.setPixelColor(
        index,
        r * brightness / MAX_VALUE,
        g * brightness / MAX_VALUE,
        b * brightness / MAX_VALUE
      );
    }
  }

    pixels.show();

    // Movimento avanti e indietro
    pos += direction;
    if (pos >= num_leds - 1 || pos <= 0) {
        //direction += -1;
        pos = 0;
    }
}

void WavePixels::moving_flash() {
  pixels.clear();

  uint8_t brightness = MAX_VALUE;
  if (--count_flash % 2 == 0) {
    brightness = 24;
  }

  // Crea la scia con effetto di dissolvenza
  for (int i = 0; i < num_leds; i++) {
    pixels.setPixelColor(
      i,
      brightness,
      brightness,
      brightness
    );
  }

  pixels.show();
}
