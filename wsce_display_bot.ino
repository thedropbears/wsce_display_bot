#include <Adafruit_NeoPixel.h>
#include <Servo.h>

Servo intake_motor;
Servo tilt_motor;
Servo shooter_motor;

Adafruit_NeoPixel *chassis_leds;
Adafruit_NeoPixel *intake_leds;
Adafruit_NeoPixel *shooter_leds;

unsigned long cycle_start_millis;

constexpr auto LIGHTS_ONLY_CYCLE_TIME = 30000ul;
constexpr auto MOTORS_CYCLE_TIME      = 15000ul;

constexpr auto INTAKE_MOTOR_PIN  = 2U;
constexpr auto TILT_MOTOR_PIN    = 3U;
constexpr auto SHOOTER_MOTOR_PIN = 4U;

constexpr auto CHASSIS_LEDS_PIN = 13U;
constexpr auto INTAKE_LEDS_PIN  = 12U;
constexpr auto SHOOTER_LEDS_PIN = 11U;

constexpr auto CHASSIS_LEDS_LENGTH = 3U * 144U;
constexpr auto INTAKE_LEDS_LENGTH  = 144U;
constexpr auto SHOOTER_LEDS_LENGTH = 247U;

constexpr auto TILT_UPPER_LIMIT_PIN = 5U;
constexpr auto TILT_LOWER_LIMIT_PIN = 6U;
constexpr auto MOTOR_ENABLE_PIN = 7U;

// All routines below take "phase" as an argument
// This is a value in the range (0,1]

double fractional(const double val) {
  return val - (int)val;
}

/* LED routines */
void rainbow(const double phase, Adafruit_NeoPixel *leds)
{
    const double resolution = 1.0 / leds->numPixels();

    for (int i = 0; i < leds->numPixels(); i++)
    {
        // remap phased hue to be within 0 and 1
        double hue_scale = (i * resolution + phase);
        hue_scale        = hue_scale - (int)hue_scale;

        // scale and set hue
        long pixel_hue = 65535 * hue_scale;
        leds->setPixelColor(i, leds->gamma32(leds->ColorHSV(pixel_hue)));
    }
    leds->show();
}

void pulse(const double phase, Adafruit_NeoPixel *leds, const unsigned char r, const unsigned char g, unsigned char b) {
    const double intensity = phase > 0.5 ? phase * 2.0: (2.0 - phase * 2.0);  // Ramp up then down
    for (int i = 0; i < leds->numPixels(); i++)
    {
      leds->setPixelColor(i, intensity * r, intensity * g, intensity * b);
    }
    leds->show();    
}
void stripes(const double phase, Adafruit_NeoPixel *leds, const unsigned char r, const unsigned char g, unsigned char b) {
    const int strip_width = 4;
    const auto n = leds->numPixels();
    const unsigned int offset = phase * n/2;
    int count = 0;
    for (int i = 0; i < n / 2; i++)
    {
      const auto idx = (i + offset) % (n/2); 
      if (count < strip_width) {
        leds->setPixelColor(idx, r, g, b);
        leds->setPixelColor(n - idx, r, g, b);
      } else {
        leds->setPixelColor(idx, 0, 0, 0);
        leds->setPixelColor(n - idx, 0, 0, 0);
      }
      count = (count + 1) % (strip_width * 3);
    }
    leds->show();    
}
void solid(Adafruit_NeoPixel *leds, const unsigned char r, const unsigned char g, const unsigned char b) {
    const auto n = leds->numPixels();
    for (int i = 0; i < n; i++)
    {
        leds->setPixelColor(i, r, g, b);
    }
    leds->show();    
}

/* Motor routines */

// Convert duty in range (-1,1) to correct number of microseconds
unsigned int throttle(const double duty)
{
    return 1500 + (duty * 500);
}

void tick_motion(const double phase)
{
    if (phase < 0.05)
    {
        // Pulse all lights as warning
        stop_all_motors();
        pulse(fractional(phase / 0.05 * 5), intake_leds, 255, 0, 0);
        solid(shooter_leds, 0, 0, 0);
        solid(chassis_leds, 100, 100, 100);
    }
    else if (phase < 0.15)
    {
        const auto segment_phase = (phase - 0.05) / 0.1;
        // Intake on
        stripes(fractional(segment_phase), intake_leds, 0, 255, 0);
        pulse(fractional(segment_phase * 5), shooter_leds, 255, 0, 0); 
        // Ramp up...
        const double duty = min(segment_phase * 2, 0.5);
        intake_motor.writeMicroseconds(throttle(duty));
        shooter_motor.writeMicroseconds(throttle(-0.1));
    }
    else if (phase < 0.25)
    {
        intake_motor.writeMicroseconds(throttle(0));
        shooter_motor.writeMicroseconds(throttle(0));
        solid(intake_leds, 0, 0, 0);
        
        // More stuff...
    }
}

void stop_all_motors()
{
    intake_motor.writeMicroseconds(throttle(0));
    tilt_motor.writeMicroseconds(throttle(0));
    shooter_motor.writeMicroseconds(throttle(0));
}

void setup()
{
    // put your setup code here, to run once
    // NeoPixel takes (pin_id, string_length) as arguments
    chassis_leds = new Adafruit_NeoPixel(CHASSIS_LEDS_LENGTH, CHASSIS_LEDS_PIN);
    intake_leds  = new Adafruit_NeoPixel(INTAKE_LEDS_LENGTH, INTAKE_LEDS_PIN);
    shooter_leds = new Adafruit_NeoPixel(SHOOTER_LEDS_LENGTH, SHOOTER_LEDS_PIN);

    pinMode(TILT_UPPER_LIMIT_PIN, INPUT_PULLUP);
    pinMode(TILT_LOWER_LIMIT_PIN, INPUT_PULLUP);
    pinMode(MOTOR_ENABLE_PIN, INPUT_PULLUP);

    intake_motor.attach(INTAKE_MOTOR_PIN);
    shooter_motor.attach(SHOOTER_MOTOR_PIN);
    tilt_motor.attach(TILT_MOTOR_PIN);

    chassis_leds->begin();
    intake_leds->begin();
    shooter_leds->begin();

    cycle_start_millis = millis();
}

void loop()
{
    const auto dt = millis() - cycle_start_millis;

    const auto motors_enabled = digitalRead(MOTOR_ENABLE_PIN) == LOW;

    if (dt < LIGHTS_ONLY_CYCLE_TIME)
    {
        // Just run the light show for the first x seconds
        const auto phase = 1.0 * dt / LIGHTS_ONLY_CYCLE_TIME;
        double rainbow_phase    = fractional(1.0 * LIGHTS_ONLY_CYCLE_TIME / 1500 * phase);  // one cycle per 1.5s
        rainbow(rainbow_phase, chassis_leds);
        rainbow(rainbow_phase, intake_leds);
        rainbow(rainbow_phase, shooter_leds);
        stop_all_motors();
    }
    else if (motors_enabled && dt < (LIGHTS_ONLY_CYCLE_TIME + MOTORS_CYCLE_TIME))
    {
        // Run the motor routine
        const auto phase = 1.0 * (dt - LIGHTS_ONLY_CYCLE_TIME) / MOTORS_CYCLE_TIME;
        tick_motion(phase);
    }
    else
    {
        // Reset the counter
        cycle_start_millis = millis();
    }
}
