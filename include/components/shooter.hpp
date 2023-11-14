#ifndef SHOOTER_H
#define SHOOTER_H

#include <Servo.h>

class Shooter {
public:
  Shooter(Servo &flywheel_motor);
  ~Shooter();
  void tick();

  void load();
  void shoot();
  void stop();

private:
  Servo *flywheel_motor_;
  unsigned int flywheel_motor_speed_;
};

#endif