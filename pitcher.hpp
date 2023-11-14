#ifndef PITCHER_H
#define PITCHER_H

#include <Servo.h>

class Pitcher {
public:
  Pitcher(Servo &motor);
  ~Pitcher();
  void tick();

  void raise();
  void lower();

private:
  bool at_lower_limit();
  bool at_upper_limit();

  unsigned int upper_limit_switch_;
  unsigned int lower_limit_switch_;

  Servo *motor_;

  unsigned int motor_speed_pulse_;
};

#endif