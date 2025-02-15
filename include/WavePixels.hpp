#ifndef WAVEPIXELS_H_
#define WAVEPIXELS_H_


#include <Adafruit_NeoPixel.h>



class WavePixels {
    public:
        constexpr static float BRIGHTNESS = 255.0f;

        WavePixels(const uint pin_led, const int num_leds_, const int brightness = 255):
            num_leds(num_leds_),
            pixels(num_leds, pin_led, NEO_GRB + NEO_KHZ800),
            state(false)
        {
        }

        void update(const unsigned int delta);
        void render();
        void clear();

        void inline toggle() { state ? turn_off(): turn_on(); };
        void inline turn_on() { state = true; }
        void inline turn_off() {
            pixels.clear();
            state = false;
        }

    private:
        int i;
        Adafruit_NeoPixel pixels;
        // Adafruit_NeoPixel pixels(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800);
        const int num_leds;
        bool state;

        float wave[30] = {
           1.0000, 0.9950, 0.9802, 0.9560, 0.9231, 0.8825, 0.8353, 0.7830,
           0.7270, 0.6690, 0.6105, 0.5527, 0.4968, 0.4438, 0.3944, 0.3492,
           0.3083, 0.2718, 0.2395, 0.2113, 0.1868, 0.1656, 0.1472, 0.1312,
           0.1173, 0.1052, 0.0945, 0.0851, 0.0767, 0.0692
        };
};

#endif // WAVEPIXELS_H_
