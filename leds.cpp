#include "leds.hpp"

AddressableLeds::AddressableLeds(unsigned int id, unsigned int led_count)
    : pixel_phase_offset(0.0), resolution_(2 * PI / led_count), last_tick_time_(0U),
      pattern_update_period_ms_((1 / (DEFAULT_PATTERN_CYCLE_FREQUENCY_HZ_ * led_count)) * 1000), led_motion_(FORWARD),
      led_count_(led_count)

{
    pixels = new Adafruit_NeoPixel(led_count, id, NEO_KHZ800 + NEO_GRB);

    pixels->begin();
    pixels->setBrightness(255);
}

AddressableLeds::~AddressableLeds()
{
    delete pixels;
}

void AddressableLeds::stop()
{
    led_motion_ = STOP;
}

void AddressableLeds::send_backward()
{
    led_motion_ = BACKWARD;
}

void AddressableLeds::send_forward()
{
    led_motion_ = FORWARD;
}

void AddressableLeds::tick()
{
    calc_rainbow_();
    pixels->show();
}

void AddressableLeds::set_frequency(double new_pattern_cycle_frequency)
{
    pattern_update_period_ms_ = (1 / (new_pattern_cycle_frequency * led_count)) * 1000
}

void AddressableLeds::calc_rainbow_()
{
    unsigned int times_to_increment = (millis() - last_tick_time_) / pattern_update_period_ms_;
    if (times_to_increment > 0)
    {
        last_tick_time_ = millis();
    }

    pixel_phase_offset += led_motion_ * times_to_increment * resolution_;

    // keep phase within pi to -pi range
    pixel_phase_offset = atan2(sin(pixel_phase_offset), cos(pixel_phase_offset));

    for (int i = 0; i < pixels->numPixels(); i++)
    {
        // remap phased hue to be within 0 and 1
        double hue_scale = (sin(i * resolution_ + pixel_phase_offset) + 1) / 2.0;

        // scale and set hue
        int pixel_hue = LED_MAX_HUE * hue_scale;
        pixels->setPixelColor(i, pixels->gamma32(pixels->ColorHSV(pixel_hue)));
    }
}