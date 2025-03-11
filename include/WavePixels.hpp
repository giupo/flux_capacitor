#ifndef WAVEPIXELS_H_
#define WAVEPIXELS_H_


#include <Adafruit_NeoPixel.h>



class WavePixels {
    public:

        constexpr static uint8_t MAX_VALUE = -1;

        enum class State: uint {
           OFF = 0,
           PULSE = 1,
           FLASH = 2
        };

        constexpr static float BRIGHTNESS = 255.0f;

        WavePixels(const uint pin_led, const int num_leds_, const int brightness = 255):
            pixels(num_leds_, pin_led, NEO_GRB + NEO_KHZ800),
            num_leds(num_leds_),
            state(State::OFF) {
        }

        void setup();
        void update(const unsigned int delta);
        void render();
        void clear();


        // states
        void pulse();
        void flash();

        inline const bool is_on() const { return state != State::OFF; }
        inline const bool is_off() const { return state == State::OFF; }

        void inline toggle() { state == State::OFF ? turn_on(): turn_off(); };
        void turn_on();
        void turn_off();

        void moving_flash();
        void moving_pulse(
          const uint8_t r,
          const uint8_t g,
          const uint8_t b,
          const uint8_t tail_size
        );

        inline const State get_state() const { return state; }

    private:
        Adafruit_NeoPixel pixels;
        // Adafruit_NeoPixel pixels(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800);
        const int num_leds;
        State state;

        int pos = 0;
        int direction = 1;
        unsigned long gap_time;
        unsigned long assigned_gap_time = 80;
        uint8_t count_flash = 5;
};

#endif // WAVEPIXELS_H_
