#ifndef ROBOT_DEFS_H
#define ROBOT_DEFS_H

const unsigned int INTAKE_MOTOR_PIN  = 2U;
const unsigned int PITCHER_MOTOR_PIN = 3U;
const unsigned int SHOOTER_MOTOR_PIN = 4U;

const unsigned int PITCHER_UPPER_LIMIT_PIN = 5U;
const unsigned int PITCHER_LOWER_LIMIT_PIN = 6U;

const unsigned int MOTOR_PULSE_STOP_     = 1500U;
const unsigned int MOTOR_PULSE_FORWARD_  = 2000U;
const unsigned int MOTOR_PULSE_BACKWARD_ = 1000U;

const unsigned int INTAKE_LEDS_PIN  = 12U;
const unsigned int CHASSIS_LEDS_PIN = 13U;
const unsigned int SHOOTER_LEDS_PIN = 11U;

const unsigned int INTAKE_LEDS_LENGTH  = 144U;
const unsigned int CHASSIS_LEDS_LENGTH = 3U * 144U;
const unsigned int SHOOTER_LEDS_LENGTH = 247;

#endif
