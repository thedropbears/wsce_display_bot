#include <Arduino.h>

#include <Servo.h>

#include "include/components/intake.hpp"
#include "include/robot_defs.hpp"

Intake::Intake(Servo &motor)
    : motor_speed_pulse_(MOTOR_PULSE_STOP_), upper_limit_switch_(PITCHER_UPPER_LIMIT_PIN),
      lower_limit_switch_(PITCHER_LOWER_LIMIT_PIN)
{
    motor_ = &motor;
    motor_->attach(INTAKE_MOTOR_PIN);
    motor_->writeMicroseconds(motor_speed_pulse_);
}

Intake::~Intake()
{
    motor_->detach();
}

void Intake::spin()
{
    motor_speed_pulse_ = MOTOR_PULSE_FORWARD_;
}

void Intake::stop()
{
    motor_speed_pulse_ = MOTOR_PULSE_STOP_;
}

void Intake::tick()
{
    motor_->writeMicroseconds(motor_speed_pulse_);
}
