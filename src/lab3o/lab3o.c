/*===========================================================================
#
# EECS461 at the University of Michigan
# Lab 3 solution (oscope)
#
# Created 2004 ericjw
#
# Revision History::
#   7-25-06 lovelljc
#   1-24-07 ericjw
#   11-6-09 wangyef
#   1-25-13 jmapatel
#   2017-03-15 RMC
#   2018-09-09 cschmotz
#   2019-01 domanico
===========================================================================*/

#include "eecs461.h"
#include "S32K144.h"
#include "adc.h"
#include "gpio.h"
#include "LPUART.h"
#include "s32_core_cm4.h"
#include "interrupt_manager.h"

#define NUM_POINTS 512

char command = '0';
static volatile uint32_t num_used;
uint16_t buffer[NUM_POINTS];


void ocIsr (void){
  writeGPIO(LED_BASE[0], LED[0], 1);
  if (num_used >= NUM_POINTS){
    DISABLE_INTERRUPTS();
  }
  else{
    buffer[num_used] = read_ADC0_single(0);
    num_used++;
  }
  writeGPIO(LED_BASE[0], LED[0], 0);
  /* Clear PIT Timer Interrupt Flag */
  LPIT0->MSR = 0b1;
}


int main(void) {
  uint16_t tmp;
  uint16_t cnt;

  initEECS461();

  // Init LPIT
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK;
  LPIT0->MCR = 0x00000001;
  LPIT0->TMR[0].TVAL = 799;
  LPIT0->MIER |= 0x1;
  LPIT0->TMR[0].TCTRL = 0x01;
  LPIT0->SETTEN |= 1;
  INT_SYS_InstallHandler(LPIT0_Ch0_IRQn, &ocIsr, (isr_t*) 0);
  INT_SYS_SetPriority(LPIT0_Ch0_IRQn, 0xC);
  INT_SYS_EnableIRQ(LPIT0_Ch0_IRQn);

  initGPDO(LED_BASE[0], LED[0]);

  LPUART1_init();

  init_ADC0_single();

  while (1) {
    DISABLE_INTERRUPTS();

    /*initialize variables*/
    num_used = 0;

    /*Check serial port for command*/
    command = LPUART1_receive_char();

    /* Enable the ISR for 50kHz sampling */
    ENABLE_INTERRUPTS();

    /* Wait for the buffer to be filled */
    while (num_used < NUM_POINTS){}

    /* send data */
    for(cnt = 0; cnt < num_used; cnt++){
      tmp = buffer[cnt];
      LPUART1_transmit_char((tmp >> 5) & 0xFF);
    }
  }

  return 0;
}
