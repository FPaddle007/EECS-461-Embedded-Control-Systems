/*
   worlds.c
   University of Michigan
   EECS 461, Embedded Control Systems
   Virtual Worlds

   Revision History
     2011-12-01  Jay Patel
     2014-02-24  Scott Kenyon
     2017-03-14  RMC
     2020-09-23  jfr
     2021-01-12  jfr
 */

#include "worlds.h"
#include "gpio.h"
#include "LPIT.h"



#define WORLD_ISR_FREQUENCY LPIT_FREQUENCY
 

float global_k=17.7;
float global_m=0.45;

//====================LAB 4================================
/***************************************************************************
 * Virtual Wall
 ***************************************************************************/
float virtualWall(float angle)
{
    float kwall = 500;
    if(angle > WALL_POSITION)
      return -(kwall) * (angle);
    else
      return 0;
}

/***************************************************************************
 * Virtual Spring
 ***************************************************************************/
float virtualSpring(float angle)
{
  float kspring = 10;
  return -(kspring) * (angle);

}


//====================LAB 6================================
/***************************************************************************
 * Virtual Spring Damper
***************************************************************************/
float virtualSpringDamper(float angle, float velocity)
{
  float torque;
  float kspring = 10.0;
  //float damp = 0.6475; //Calculated spring damp from lab 6 q1
  float damp = 0.4;
  torque = -kspring*angle - damp*velocity;
  return torque;
}

/***************************************************************************
 * Virtual Wall Damper
***************************************************************************/
float virtualWallDamper(float angle, float velocity)
{
  if(angle > WALL_POSITION){
    float torque;
    float kwall = 500.0; //Dealing in N-m not N-mm
    //float damp = 0.25;
    float damp = 5;
    torque = -kwall*angle - damp*velocity;
    return torque;
    }
  else{
    return 0;
  }
}

/***************************************************************************
 * Virtual Spring Mass
***************************************************************************/
float virtualSpringMass(float angle)
{
  static float thetaw_old, thetaw_velocity_old;
  //float k = 17.7/1000;
  //float J = 0.45/1000;
  //float b = 17.7/1000000;
  float k = 17.7;
  float J = 0.45;
  float b = 17.7/1000;
  float T = 1/WORLD_ISR_FREQUENCY;

  float thetaw = 0;
  float thetaw_velocity = 0;


  //Update the thetaw stuff
  thetaw = thetaw_old + T*thetaw_velocity_old;
  thetaw_velocity = -1*(thetaw_old)*((k*T)/J) + thetaw_velocity_old + angle*((k*T)/J);

  //Calculate torque
  float torque = k*(thetaw-angle);

  //Store the old stuff
  thetaw_old = thetaw;
  thetaw_velocity_old = thetaw_velocity;

  return torque;

}

/***************************************************************************
 * Virtual Spring Mass Damper
***************************************************************************/
float virtualSpringMassDamper(float angle, float velocity)
{
  static float thetaw_old, thetaw_velocity_old;
  //float k = 17.7/1000;
  //float J = 0.45/1000;
  //float b = 17.7/1000000;
  float k = global_k;
  float J = global_m;
  float b = 17.7/1000;
  float T = 1/WORLD_ISR_FREQUENCY;

  float thetaw = 0;
  float thetaw_velocity = 0;

  //Update the thetaw stuff
  thetaw = thetaw_old + thetaw_velocity_old;
  thetaw_velocity = -1*(thetaw_old)*(k*T/J) + (1 - (b*T/J))*thetaw_velocity_old + angle*(k*T/J) +
  (b*T/J)*velocity;

  //Calculate torque
  float torque = k*(thetaw-angle) + b*(thetaw_velocity - velocity);

  //Store the old stuff
  thetaw_old = thetaw;
  thetaw_velocity_old = thetaw_velocity;

  return torque;
}

/***************************************************************************
 * Virtual Knob
***************************************************************************/
float virtualKnob(float angle, float velocity)
{
  //There are many ways this can be implemented

  static int i = 0;
  static float adjusted_angle = 0;

  adjusted_angle = angle - 45*i;

  //Every 45 degrees, we hit a wall that the puck is trying to avoid
  if(adjusted_angle < 45){
    float torque;
    float kwall = 100.0;
    float damp = 0.25;

    torque = -kwall*adjusted_angle - damp*velocity;
    return torque;

  }

  else if(adjusted_angle == 45){
    i++;
    return 0;
  }
  else{
    return 0;
  }
}
