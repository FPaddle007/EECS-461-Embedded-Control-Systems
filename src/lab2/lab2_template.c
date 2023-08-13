/*
  EECS461
  Lab 2
  Updated: March 17th, 2014 by Scott Kenyon
           July  4th, 2014 by GPCz
           2017-03-15 RMC
           2018-03    Paul Domanico
           2021-01-12  jfr
           2021-09-02  jfr
           2021-09-17  jfr
           2022-08-12  jfr
*/


 #include "eecs461.h"
 #include "S32K144.h"
 #include "gpio.h"
 #include "qd.h"


 int main(void){
   uint16_t counter = 0;
   int32_t total = 0;
   int i;
   float angle;
   int16_t angleInt;
   int section = 1; //Set to case you are testing

   /* Initialize EECS461, QD, GPDO */

   while(1){
     switch(section){
     case 1://Read the counter
       /* fill in */
       break;

     case 2://Overflow and Underflow
       /* fill in */
       break;

     case 3: //updateCounter and Faulty Casting
       /* fill in */
       break;

     case 4: //Angle Calculation
       /* fill in */
       break;
     }
   }
 }
