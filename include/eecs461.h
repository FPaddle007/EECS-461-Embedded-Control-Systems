/*
 * eecs461.h
 *
 *  Created on: Jan 26, 2018
 *      Author: domanico
 */

#ifndef EECS461_H_
#define EECS461_H_

/* Clock Frequencies */
#define SPLL_CLK_FREQ      160000000u  /* 160 MHz */
#define SYS_CLK_FREQ       80000000u   /* 80 MHz */
#define BUS_CLK_FREQ       40000000u   /* 40 MHz */
#define SPLLDIV2_CLK_FREQ  40000000u   /* 40 MHz */
#define SPLLDIV1_CLK_FREQ  10000000u   /* 10 MHz */
#define SOSC_CLK_FREQ      8000000u    /* 8 MHz */
#define SOSCDIV1_CLK_FREQ  8000000u    /* 8 MHz */
#define SOSCDIV2_CLK_FREQ  8000000u    /* 8 MHz */

/*
 * Function (MACRO):  ASSERT_EECS461
 * Description:       Simple assert to use instead of the assert.h stl. If the input evaluates
 *                    to zero, a breakpoint is triggered.
 * Param: x           Value or expression that may evaluate to 0
 */
#ifndef NDEBUG
# define ASSERT_EECS461(x)   if((x) == 0) __asm volatile("BKPT")
#else
# define ASSERT_EECS461(x)
#endif

void initEECS461(void);

void clocksToPeripherals(void);

void disable_WDOG(void);

void setEVBLED(void);

void SOSC_init_8MHz (void);
void SPLL_init_160MHz (void);
void NormalRUNmode_80MHz (void);

void NMI_Handler();
void HardFault_Handler();
void MemManage_Handler();
void BusFault_Handler();
void UsageFault_Handler();

#endif /* EECS461_H_ */
