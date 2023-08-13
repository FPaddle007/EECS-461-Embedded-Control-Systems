/*
   pwm.h
   University of Michigan
   EECS 461, Embedded Control Systems
   Pulse-Width Modulation using FlexTimer Module
   Read Chapter 45 in S32K144 User's Manual
   Section 45.5.7: Edge-Aligned PWM (EPWM) Mode
 */
#ifndef PWM_H_
#define PWM_H_

#include "S32K144.h"

// Motor and Filter Constants
#define MOTOR_FREQUENCY 20000
#define MOTOR_SUBMODULE 0
#define MOTOR_CHANNEL 0

#define FILTER_FREQUENCY 1000000
#define FILTER_SUBMODULE 3
#define FILTER_CHANNEL 0

#define DC_UPPER_LIMIT 0.76
#define DC_LOWER_LIMIT 0.24

/* PWM Parameters */
#define PWM_CLOCK_FREQ  SPLLDIV1_CLK_FREQ

#define MOTOR_PORT PORTB
#define MOTOR_PCR 12
#define MOTOR_MUX 0b00000010

#define FILTER_PORT PORTB
#define FILTER_PCR 8
#define FILTER_MUX 0b00000010

#define FILTER_DUTY_CYCLE 0.5f

// Direct access to PWM registers
extern FTM_Type * FTM_MODULE[4];

/* Functions for PWM */
void initPWMPCRs();
void setPWM(const int submodule, const int channel, const int frequency, const float dutyCycle);
void initPWM(const int submodule, const int channel, const int frequency, const float dutyCycle);
void outputTorque(const float torque);

#endif /* PWM_H_ */
