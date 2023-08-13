/*
 EECS461
 Lab 6
*/
#include "S32K144.h"
#include "eecs461.h"
#include "s32_core_cm4.h"
#include "LPIT.h"
#include "LPUART.h"
#include "qd.h"
#include "pwm.h"
#include "worlds.h"


void wallDamper(void)
{
/* ISR for the wall-damper system */
 static float velocity;
 static float old_angle = 0;
 static float angle;

 float T = 1.0/WORLDISR_FREQUENCY;


 //Update values:
 old_angle = angle;
 angle = updateAngle();
 velocity = (angle - old_angle)/T;

 //Set Torque
 float torque = virtualWallDamper(angle, velocity);
 outputTorque(torque);

 /* Make sure to clear interrupt flag */
 /* Clear interrupt flag */
 clearFlagLPIT(LPIT0_CHANNEL);
}

void springDamper(void)
{
/* ISR for the spring-damper system */
 static float velocity;
 static float old_angle = 0;
 static float angle;
 float T = 1.0/WORLDISR_FREQUENCY;

//Update values:
 old_angle = angle;
 angle = updateAngle();
 velocity = (angle-old_angle)/T;

//Set torque
 float torque = virtualSpringDamper(angle, velocity);
 outputTorque(torque);

/* Make sure to clear interrupt flag */
 /* Clear interrupt flag */
 clearFlagLPIT(LPIT0_CHANNEL);
}

void springMass(void)
{
/* ISR for the spring-mass system */
 static float angle;
 angle = updateAngle();

float torque = virtualSpringMass(angle);
 outputTorque(torque);
 /* Make sure to clear interrupt flag */
 /* Clear interrupt flag */
 clearFlagLPIT(LPIT0_CHANNEL);
}

void springMassDamper(void)
{
/* ISR for the spring-mass-damper system */
 static float velocity;
 static float old_angle = 0;
 static float angle;

 float T = 1.0/WORLDISR_FREQUENCY;

 //Update values:
 old_angle = angle;
 angle = updateAngle();
 velocity = (angle-old_angle)/T;

 float torque = virtualSpringMassDamper(angle, velocity);
 outputTorque(torque);

 /* Make sure to clear interrupt flag */
 /* Clear interrupt flag */
clearFlagLPIT(LPIT0_CHANNEL);
}

void knobIndents(void)
{
/* ISR for the virtual knob system */
 static float velocity;
 static float old_angle = 0;
 static float angle;
 float T = 1.0/WORLDISR_FREQUENCY;

 //Update values:
 old_angle = angle;
 angle = updateAngle();
 velocity = (angle-old_angle)/T;

 float torque = virtualKnob(angle, velocity);
 outputTorque(torque);
 /* Make sure to clear interrupt flag */
 /* Clear interrupt flag */
 clearFlagLPIT(LPIT0_CHANNEL);
}

int main(void) {
  char byte_in = 0;
  void (*interrupt)(void);
  int section = 6;

  initEECS461();
  initQD();
  enableLPIT();
  LPUART1_init();

  // Initialize PWMs
  initPWMPCRs();

  initPWM(MOTOR_SUBMODULE, MOTOR_CHANNEL, MOTOR_FREQUENCY, 0.5f);
  initPWM(FILTER_SUBMODULE, FILTER_CHANNEL, FILTER_FREQUENCY, 0.5f);

  if(section == 5)
  {
    LPUART1_init();
    LPUART1_transmit_string("\n\n\n\n\n\n\n\rSerial Output Enabled.");
  }

  switch(section)
  {
   case 1:
     interrupt = &wallDamper;
     break;

   case 2:
      interrupt = &springDamper;
      break;

   case 3:
     interrupt = &springMass;
     break;

   case 4:
     interrupt = &springMassDamper;
     break;

   case 5:
     interrupt = &springMassDamper;
     break;

   case 6:
     interrupt = &knobIndents;
     break;
  }

  //Fill in the arguments to initialize the appropriate interrupt routine for
  //each part of the lab. Be sure to use the correct channel specified in worlds.h
  initLPIT(LPIT0_CHANNEL, WORLDISR_FREQUENCY, interrupt, 0xC);

  while(1){
    if(section == 5){
      byte_in = LPUART1_receive_char();
      DISABLE_INTERRUPTS();
      /* adjust K and M from the keyboard. Must be a critical section. */
      switch(byte_in)
      {
       case 'k':
         global_k=global_k*1.1;
         break;

       case 'K':
         global_k=global_k/1.1;
         break;

       case 'm':
         global_m=global_m*1.1;
         break;

       case 'M':
         global_m=global_m/1.1;
         break;

       default:
         break;
      }
      ENABLE_INTERRUPTS();
    }
  }
}
