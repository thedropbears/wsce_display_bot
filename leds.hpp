#ifndef ADDRESSABLE_LEDS_H
#define ADDRESSABLE_LEDS_H

#include <Adafruit_NeoPixel.h>

class AddressableLeds
{

public:
    AddressableLeds(unsigned int id, unsigned int led_count);
    ~AddressableLeds();

    void send_forward();
    void send_backward();
    void stop();

    void set_frequency(double new_pattern_cycle_frequency);

    void tick();

private:
    void calc_rainbow_();

    Adafruit_NeoPixel *pixels;

    typedef enum
    {
        BACKWARD = -1,
        STOP     = 0,
        FORWARD  = 1
    } AddressableLedsMotion;

    AddressableLedsMotion led_motion_;

    double             pixel_phase_offset;
    double             resolution_;
    unsigned long long last_tick_time_;
    unsigned long long pattern_update_period_ms_;
    unsigned int       led_count_;

    const double       DEFAULT_PATTERN_CYCLE_FREQUENCY_HZ_ = 2.0;
    const unsigned int LED_MAX_HUE                         = 65535U;
};

#endif