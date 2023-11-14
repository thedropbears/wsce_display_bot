#include <Arduino.h>

#include "include/components/shooter.hpp"
#include "include/robot_defs.hpp"

Shooter::Shooter(Servo &flywheel_motor) : flywheel_motor_speed_(MOTOR_PULSE_STOP_)
{
    flywheel_motor_ = &flywheel_motor;
    flywheel_motor_->attach(SHOOTER_MOTOR_PIN);
}

Shooter::~Shooter()
{
    flywheel_motor_->detach();
}

void Shooter::load()
{
    flywheel_motor_speed_ = MOTOR_PULSE_BACKWARD_;
}

void Shooter::shoot()
{
    flywheel_motor_speed_ = MOTOR_PULSE_FORWARD_;
}

void Shooter::stop()
{
    flywheel_motor_speed_ = MOTOR_PULSE_STOP_;
}

void Shooter::tick()
{
    flywheel_motor_->writeMicroseconds(flywheel_motor_speed_);
}
