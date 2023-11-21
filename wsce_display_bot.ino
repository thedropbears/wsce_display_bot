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

// All routines below take "phase" as an argument
// This is a value in the range (0,1]

/* LED routines */
void rainbow(const double phase, Adafruit_NeoPixel *leds) {}

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
    }
    else if (phase < 0.1)
    {
        // Intake on
        // Ramp up...
        const double duty = (phase - 0.05) / 0.05;
        intake_motor.writeMicroseconds(throttle(duty));
    }
    else if (phase < 0.25)
    {
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

    intake_motor.attach(INTAKE_MOTOR_PIN);
    shooter_motor.attach(SHOOTER_MOTOR_PIN);
    tilt_motor.attach(TILT_MOTOR_PIN);

    cycle_start_millis = millis();
}

void loop()
{
    const auto dt = millis() - cycle_start_millis;

    const auto motors_enabled = false; // TODO - read the switch

    if (dt < LIGHTS_ONLY_CYCLE_TIME)
    {
        // Just run the light show for the first x seconds
        const auto phase = 1.0 * dt / LIGHTS_ONLY_CYCLE_TIME;
        rainbow(phase, chassis_leds);
        rainbow(phase, intake_leds);
        rainbow(phase, shooter_leds);
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
