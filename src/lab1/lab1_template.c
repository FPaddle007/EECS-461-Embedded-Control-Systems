/*
  EECS461
  Lab 1
  Read Chapter 11&12 in S32K144 User's Manual
  Updated: March 17th, 2014 by Scott Kenyon
           14 Jun 2014 by GPCz
           19 Jun 2014 by GPCz
           2017-03 RMC
           2018-03 Paul Domanico
           15 Nov 2018 jfr
           12 Jan 2021 jfr
           29 Aug 2021 jfr
           12 Aug 2022 jfr
*/

#include "mylab1header.h"
#include "eecs461.h"
#include <stdint.h>

/* Change section number to match the section (1-3) being tested */
#define SECTION 2

#if SECTION == 1



/******************************************************************************
 * Function:    section1
 * Description: read and write to the GPIO registers using your header file
 ******************************************************************************/
void section1(){
  volatile uint32_t * const portD_PCR = (uint32_t *)(BASE_PORTD);
  volatile uint32_t * const portE_PCR = (uint32_t *)(BASE_PORTE);
  GPIO_mem * const gpioD = (GPIO_mem *)(BASE_GPIO + GPIOD_OFFSET);
  GPIO_mem * const gpioE = (GPIO_mem *)(BASE_GPIO + GPIOE_OFFSET);
  uint16_t sum, value1, value2;
  uint32_t regReadVal;
  int index;

  /* Configure the LED pins */
  for(index = 0; index < 16; index++){
    /* Configure pin mux to gpio */
    portD_PCR[index] = 0b001 << 8;
    /* Set the direction to output */
    gpioD->PDDR |= 0b1 << index;
  }
  /* Configure the DIP pins */
  for(index = 6; index < 14; index++){
    /* Configure pin mux to gpio */
    portE_PCR[index] = 0b001 << 8;
    /* Set the direction to input */
    gpioE->PDDR &= ~(1 << index);
  }
  while(1){
    value1 = 0;
    value2 = 0;
    sum = 0;
    /* Read the GPIOE input register, extract the two 4 bit numbers
    set on dipswitches 1-4 and 5-8  */
    regReadVal = (gpioE->PDIR) >> 6;
    value1=regReadVal & 0x0F;
    value2=(regReadVal & 0xF0) >> 4;

    /* Calculate the sum and write to the GPIOD output register
    to display the results on LEDs1-5.  */
    sum = value1+value2;
    for(index = 0; index < 5; index++){
      uint16_t mask = 1<<index;
      if( (sum & mask) > 0 )
        gpioD->PDOR |= mask;  // set bit to 1
      else
        gpioD->PDOR &= ~mask; // set bit to 0
    }


  }
}


#elif SECTION == 2
#include "S32K144.h"
#include "gpio.h"
/******************************************************************************
 * Function:    section2
 * Description: Use the provided S32K144.h header file
 ******************************************************************************/
void section2(){
  /* code for section 2 */

}


#elif SECTION == 3
#include "LPUART.h"
#include "gpio.h"
/******************************************************************************
 * Function:    section3
 * Description: Communicate with the computer using the keyboard interface
 ******************************************************************************/
void section3(){
  /* code for section 3 */
  char byte_in, old_byte_in;

  /* LPUART functions for sending and receiving serial data */
  LPUART1_init();
  LPUART1_transmit_string("\n\rSerial Output Enable...........");
  while(1) {
    old_byte_in = byte_in;
    byte_in = LPUART1_receive_char(); // returns ASCII representation


  }
}
#endif

int main(){
  /* Always call initEECS461() before doing anything else */
  initEECS461();

  #if(SECTION == 1)
  section1();
  #elif(SECTION == 2)
  section2();
  #elif(SECTION == 3)
  section3();
  #endif

  /* Should never reach this point */
  return 0;
}
