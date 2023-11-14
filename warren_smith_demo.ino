
#include "include/components/intake.hpp"
#include "include/components/pitcher.hpp"
#include "include/components/shooter.hpp"
#include "include/robot_defs.hpp"

typedef enum
{
    ACTUATE_INTAKE = 0,
    ACTUATE_SHOOTER,
    ACTUATE_PITCHER
} ActuationState;

Servo shooter_flywheel;
Servo intake_motor;
Servo pitcher_motor;

Intake  *intake;  //{intake_motor};
Shooter *shooter; //{shooter_flywheel};
Pitcher *pitcher; //{pitcher_motor};

unsigned long last_state_transition_time;
unsigned long last_tick_time;

ActuationState state;

void intake_callback()
{
    shooter->stop();
    pitcher->lower();

    intake->spin();
}

void shooter_callback()
{
    pitcher->lower();
    intake->stop();

    shooter->shoot();
}

void pitcher_callback()
{
    intake->stop();
    shooter->stop();

    pitcher->raise();
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

    Serial.begin(9600);
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

        last_tick_time = millis();
        Serial.println(last_tick_time);
    }
}
