#ifndef WAVEPIXELS_H_
#define WAVEPIXELS_H_


#include <Adafruit_NeoPixel.h>



class WavePixels {
    public:
        constexpr static float BRIGHTNESS = 255.0f;

        WavePixels(const uint pin_led, const int num_leds_, const int brightness = 255):
            pixels(num_leds_, pin_led, NEO_GRB + NEO_KHZ800),
            num_leds(num_leds_),
            state(false) {
        }

        void setup();
        void update(const unsigned int delta);
        void render();
        void clear();

        inline const bool is_on() const { return state == true; }
        inline const bool is_off() const { return state == false; }

        void inline toggle() { state ? turn_off(): turn_on(); };
        void turn_on();
        void turn_off();

        void moving_pulse(unsigned char r, unsigned char g, unsigned char b, unsigned char brightness);
        void moving_trail(uint8_t r, uint8_t g, uint8_t b, const uint8_t tail_size);

        inline const bool get_state() const { return state; }

    private:
        int i;
        Adafruit_NeoPixel pixels;
        // Adafruit_NeoPixel pixels(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800);
        const int num_leds;
        bool state;

        int pos = 0;
        int direction = 1;
        unsigned int gap_time;
        constexpr static auto GAP_TIME = 150;
};

#endif // WAVEPIXELS_H_
