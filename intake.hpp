#ifndef INTAKE_H
#define INTAKE_H

#include <Servo.h>

class Intake {
public:
  Intake(Servo &motor);
  ~Intake();
  void tick();

  void spin();
  void stop();

private:
  unsigned int upper_limit_switch_;
  unsigned int lower_limit_switch_;

  Servo *motor_;

  unsigned int motor_speed_pulse_;
};

#endif