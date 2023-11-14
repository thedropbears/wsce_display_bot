#include <Arduino.h>

#include "include/components/pitcher.hpp"
#include "include/robot_defs.hpp"

Pitcher::Pitcher(Servo &motor) : motor_speed_pulse_(MOTOR_PULSE_STOP_) {
  motor_ = &motor;
  motor_->attach(PITCHER_MOTOR_PIN);
}

Pitcher::~Pitcher() {
  motor_->detach();
  // delete motor_;
}

void Pitcher::raise() { motor_speed_pulse_ = MOTOR_PULSE_BACKWARD_; }

void Pitcher::lower() { motor_speed_pulse_ = MOTOR_PULSE_FORWARD_; }

void Pitcher::tick() {

  if ((at_lower_limit()) || (at_upper_limit())) {
    motor_speed_pulse_ = MOTOR_PULSE_STOP_;
  }

  motor_->writeMicroseconds(motor_speed_pulse_);
}

bool Pitcher::at_lower_limit() { return digitalRead(lower_limit_switch_); }

bool Pitcher::at_upper_limit() { return digitalRead(upper_limit_switch_); }
