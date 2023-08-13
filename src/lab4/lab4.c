/*
  EECS461
  Lab 4
   Revision History
     2020-09-23  jfr
     2021-01-12  jfr
     2022-08012  jfr
*/
#include "eecs461.h"
#include "qd.h"
#include "pwm.h"
#include "worlds.h"
#include "gpio.h"
#include "adc.h"

#define MAX_ADC_VAL 5000
#define MIN_ADC_VAL 0
#define DC_UPPER_LIMIT 0.76
#define DC_LOWER_LIMIT 0.24
#define ADC_TO_DUTY_SCALAR ((DC_UPPER_LIMIT - DC_LOWER_LIMIT) / (MAX_ADC_VAL - MIN_ADC_VAL))
#define ADC_TO_DUTY_OFFSET (DC_LOWER_LIMIT)

int main(void)
{
  int section = 6;
  uint32_t adc = 0;
  float duty = 0;
  float angle = 0;
  float torque = 0;

  initEECS461();
  
  // Initialize QD and ADC
  initQD();
  init_ADC0_single();

  // Initialize PWMs
  initPWMPCRs();
  initPWM(MOTOR_SUBMODULE, MOTOR_CHANNEL, MOTOR_FREQUENCY, 0.5);
  initPWM(FILTER_SUBMODULE, FILTER_CHANNEL, FILTER_FREQUENCY, FILTER_DUTY_CYCLE);

  while(1)
  {
    switch(section)
    {
      case 1: // 0 Torque
        outputTorque(0);
        break;
      case 2: //Analog Input Control of PWM
        adc = read_ADC0_single(0);
        duty = ADC_TO_DUTY_OFFSET + (adc*ADC_TO_DUTY_SCALAR);
        setPWM(MOTOR_SUBMODULE, MOTOR_CHANNEL, MOTOR_FREQUENCY, duty);
        break;
      case 3: //Change PWM Frequency to 1 kHz torque = 0 N-mm
        outputTorque(0);
        break;
      case 4: //Set torque to 200 N-mm
        outputTorque(200);
        break;
      case 5: //Virtual Spring
        angle=updateAngle();
        torque=virtualSpring(angle);
        outputTorque(torque);
        break;
      case 6: // Virtual Wall
        angle=updateAngle();
        torque=virtualWall(angle);
        outputTorque(torque);
        break;
    }
  }
}

