/*
  EECS461
  Lab 3
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
 #include "adc.h"


int main(void)
{
   initEECS461();
   init_ADC0_single();

   //section 4.2
/*
   int i = 0;
   initGPDO(LED_BASE[0], LED[0]);

   while(1){
     writeGPIO(LED_BASE[0], LED[0], 1);
     uint32_t iAnalog = read_ADC0_single(0);

     writeGPIO(LED_BASE[0], LED[0], 0);
   }
   */

   /*
   // section 4.3
   int i = 0;
       initGPDO(LED_BASE[0], LED[0]);

       while(1){

         uint32_t iAnalog = read_ADC0_single(0);

         if(iAnalog >= 2500){
           writeGPIO(LED_BASE[0], LED[0], 1);
         }
         else{
         writeGPIO(LED_BASE[0], LED[0], 0);
         }

       }
       */

}
