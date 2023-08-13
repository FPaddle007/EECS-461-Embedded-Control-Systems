/*
 * eecs461.c
 *
 *  Created on: Jan 26, 2018
 *      Author: domanico
 */

#include "eecs461.h"
#include "S32K144.h"

/******************************************************************************
 * Function:	initEECS461
 * Description:	Perform necessary initializations for the processor.
 * 				This function should be called before all student-written code.
 ******************************************************************************/
void initEECS461(void){
	disable_WDOG();

	/* Initialize clocks using "clocks_and_modes" */
	SOSC_init_8MHz();
	SPLL_init_160MHz();
	NormalRUNmode_80MHz();

	clocksToPeripherals();

	setEVBLED();
}

/******************************************************************************
 * Function:	disable_WDOG
 * Description:	Disable watchdog timer
 ******************************************************************************/
void disable_WDOG(void){
	WDOG->CNT=0xD928C520; 	/*Unlock watchdog*/
	WDOG->TOVAL=0x0000FFFF;	/*Maximum timeout value*/
	WDOG->CS = 0x00002100;  /*Disable watchdog*/
}

/******************************************************************************
 * Function: clocksToPeripherals
 * Description: Enable clocks to peripherals (PORT modules)
 ******************************************************************************/
void clocksToPeripherals(void){
    PCC->PCCn[PCC_PORTA_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT A */
    PCC->PCCn[PCC_PORTB_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT B */
    PCC->PCCn[PCC_PORTC_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT C */
    PCC->PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT D */
    PORTD->PCR[3] = (uint32_t) 0; /* Disable the NMI */
    PCC->PCCn[PCC_PORTE_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT E */

    /* Enable clock to FTM2 */
    PCC->PCCn[PCC_FTM2_INDEX] &= ~PCC_PCCn_CGC_MASK; 	/* Ensure clk disabled for config */
    PCC->PCCn[PCC_FTM2_INDEX] |= PCC_PCCn_PCS(0b110)	/* Clock Src=6, 10 MHz SPLLDIV1_CLK_FREQ */
                                 | PCC_PCCn_CGC_MASK; /* Enable clock for FTM regs */

    /* Enable clock to FTM0 */
    PCC->PCCn[PCC_FTM0_INDEX] &= ~PCC_PCCn_CGC_MASK; 	/* Ensure clk disabled for config */
    PCC->PCCn[PCC_FTM0_INDEX] |= PCC_PCCn_PCS(0b110)	/* Clock Src=6, 10 MHz SPLLDIV1_CLK_FREQ */
                                 | PCC_PCCn_CGC_MASK; /* Enable clock for FTM regs */

    /* Enable clock to FTM3 */
    PCC->PCCn[PCC_FTM3_INDEX] &= ~PCC_PCCn_CGC_MASK; 	/* Ensure clk disabled for config */
    PCC->PCCn[PCC_FTM3_INDEX] |= PCC_PCCn_PCS(0b110)	/* Clock Src=6, 10 MHz SPLLDIV1_CLK_FREQ */
                                 | PCC_PCCn_CGC_MASK; /* Enable clock for FTM regs */
}

/******************************************************************************
 * Function: setEVBLED
 * Description: Set the Eval-board LEDs low.  This will avoid inadvertendly
 *              turning on the LEDs on the IO board but turn on the LEDs on the
 *              EVB.  (active high on IO board, active low on EVB)
 ******************************************************************************/
void setEVBLED(void){
    int pad_green = 16, pad_blue = 0, pad_red = 15;

    // Pin Mux Alt1 for GPIO
    PORTD->PCR[pad_green] &= ~(0b111 << PORT_PCR_MUX_SHIFT);
    PORTD->PCR[pad_green] |= 0b001 << PORT_PCR_MUX_SHIFT;
    PORTD->PCR[pad_blue] &= ~(0b111 << PORT_PCR_MUX_SHIFT);
    PORTD->PCR[pad_blue] |= 0b001 << PORT_PCR_MUX_SHIFT;
    PORTD->PCR[pad_red] &= ~(0b111 << PORT_PCR_MUX_SHIFT);
    PORTD->PCR[pad_red] |= 0b001 << PORT_PCR_MUX_SHIFT;

    // Set the pad direction to output
    PTD->PDDR |= ((0b1 << pad_green) | (0b1 << pad_blue) | (0b1 << pad_red));

    // Active low led
    PTD->PCOR |= ((0b1 << pad_green) | (0b1 << pad_blue) | (0b1 << pad_red));
}


/******************************************************************************
 * Function: SOSC_init_8MHz, SPLL_init_160MHz, NormalRUNmode_80MHz
 * Description: Setup the system clocks
 *
 * Based on NXP example code clocks_and_modes.c
 * 2015 Feb 22 S Mihalik/ O Romero - initial version
 * 2016 Oct 15 S.M. / O.R. - Updated PLL per S32K1XX data sheet rev 1
 ******************************************************************************/

void SOSC_init_8MHz(void) {
  SCG->SOSCDIV=0x00000101;  /* SOSCDIV1 & SOSCDIV2 =1: divide by 1 */
  SCG->SOSCCFG=0x00000024;  /* Range=2: Medium freq (SOSC betw 1MHz-8MHz)*/
                            /* HGO=0:   Config xtal osc for low power */
                            /* EREFS=1: Input is external XTAL */
  while(SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK); /* Ensure SOSCCSR unlocked */
  SCG->SOSCCSR=0x00000001;  /* LK=0:          SOSCCSR can be written */
                            /* SOSCCMRE=0:    OSC CLK monitor IRQ if enabled */
                            /* SOSCCM=0:      OSC CLK monitor disabled */
                            /* SOSCERCLKEN=0: Sys OSC 3V ERCLK output clk disabled */
                            /* SOSCLPEN=0:    Sys OSC disabled in VLP modes */
                            /* SOSCSTEN=0:    Sys OSC disabled in Stop modes */
                            /* SOSCEN=1:      Enable oscillator */
  while(!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK)); /* Wait for sys OSC clk valid */
}

void SPLL_init_160MHz(void) {
  while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */
  SCG->SPLLCSR = 0x00000000;  /* SPLLEN=0: SPLL is disabled (default) */
  SCG->SPLLDIV = 0x00000305;  /* SPLLDIV1 divide by 16; SPLLDIV2 divide by 4 */
  SCG->SPLLCFG = 0x00180000;  /* PREDIV=0: Divide SOSC_CLK by 0+1=1 */
                              /* MULT=24:  Multiply sys pll by 4+24=40 */
                              /* SPLL_CLK = 8MHz / 1 * 40 / 2 = 160 MHz */
  while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */
  SCG->SPLLCSR = 0x00000001; /* LK=0:        SPLLCSR can be written */
                             /* SPLLCMRE=0:  SPLL CLK monitor IRQ if enabled */
                             /* SPLLCM=0:    SPLL CLK monitor disabled */
                             /* SPLLSTEN=0:  SPLL disabled in Stop modes */
                             /* SPLLEN=1:    Enable SPLL */
  while(!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK)); /* Wait for SPLL valid */
}

void NormalRUNmode_80MHz (void) {  /* Change to normal RUN mode with 8MHz SOSC, 80 MHz PLL*/
  SCG->RCCR=SCG_RCCR_SCS(6)      /* PLL as clock source*/
    |SCG_RCCR_DIVCORE(0b01)      /* DIVCORE= 2, Core clock = 160/2 MHz = 80 MHz*/
    |SCG_RCCR_DIVBUS(0b01)       /* DIVBUS = 2, bus clock = 40 MHz*/
    |SCG_RCCR_DIVSLOW(0b10);     /* DIVSLOW = 4, SCG slow, flash clock= 20 MHz*/
  while (((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT ) != 6) {}
                                 /* Wait for sys clk src = SPLL */
}


/******************************************************************************
 * Function: <fault>_Handler
 * Description: Define infinite loops for these possible faults so that we
 *              can determine what kind of fault occurred during debugging
 ******************************************************************************/
void NMI_Handler(){
  __asm volatile("BKPT");
  while(1);
}
void HardFault_Handler(){
  /* Did you call initEECS461() BEFORE doing anything else?? */
  __asm volatile("BKPT");
  while(1);
}
void MemManage_Handler(){
  __asm volatile("BKPT");
  while(1);
}
void BusFault_Handler(){
  __asm volatile("BKPT");
  while(1);
}
void UsageFault_Handler(){
  __asm volatile("BKPT");
  while(1);
}
