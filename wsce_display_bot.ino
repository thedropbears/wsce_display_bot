
#include "intake.hpp"
#include "pitcher.hpp"
#include "shooter.hpp"
#include "robot_defs.hpp"
#include "leds.hpp"

typedef enum
{
    ACTUATE_INTAKE = 0,
    ACTUATE_SHOOTER,
    ACTUATE_PITCHER
} ActuationState;

Servo shooter_flywheel;
Servo intake_motor;
Servo pitcher_motor;

Intake          *intake;  //{intake_motor};
Shooter         *shooter; //{shooter_flywheel};
Pitcher         *pitcher; //{pitcher_motor};
AddressableLeds *leds;

unsigned long last_state_transition_time;
unsigned long last_tick_time;

ActuationState state;

void intake_callback()
{
    shooter->stop();
    pitcher->lower();

    intake->spin();
    leds->send_forward();
}

void shooter_callback()
{
    pitcher->lower();
    intake->stop();

    shooter->shoot();
    leds->send_backward();
}

void pitcher_callback()
{
    intake->stop();
    shooter->stop();

    pitcher->raise();
    leds->stop();
}

void setup()
{
    // put your setup code here, to run once
    last_state_transition_time = 0U;
    state                      = ACTUATE_INTAKE;
    last_tick_time             = 0U;

    intake  = new Intake(intake_motor);
    shooter = new Shooter(shooter_flywheel);
    pitcher = new Pitcher(pitcher_motor);
    leds    = new AddressableLeds(CHASSIS_LEDS_PIN, CHASSIS_LEDS_LENGTH);
    leds->set_frequency(5.0);
}

void loop()
{
    // put your main code here, to run repeatedly:
    if ((millis() - last_tick_time) > 200)
    {
        if ((millis() - last_state_transition_time) > 5000U)
        {
            switch (state)
            {
                case ACTUATE_INTAKE:
                    state = ACTUATE_PITCHER;
                    break;

                case ACTUATE_PITCHER:
                    state = ACTUATE_SHOOTER;
                    break;

                case ACTUATE_SHOOTER:
                    state = ACTUATE_INTAKE;
                    break;

                default:
                    break;
            }
            last_state_transition_time = millis();
        }

        switch (state)
        {
            case ACTUATE_INTAKE:
                intake_callback();
                break;

            case ACTUATE_PITCHER:
                pitcher_callback();
                break;

            case ACTUATE_SHOOTER:
                shooter_callback();
                break;

            default:
                break;
        }

        // Tick all the things
        intake->tick();
        shooter->tick();
        pitcher->tick();
        leds->tick();

        last_tick_time = millis();
    }
}
