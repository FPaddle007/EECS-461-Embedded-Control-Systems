/*
 * lab5.c
 *
 *  Created on: Jan 21, 2018
 *  	Author: domanico

	Revision History:
	2020-09-27  jfr
	2021-01-12  jfr
	2021-09-30  jfr
	2022-08-12  jfr
 */

#include "S32K144.h"
#include "eecs461.h"

#include "pwm.h"
#include "LPIT.h"
#include "adc.h"
#include "gpio.h"
#include "math.h"

// Interrupt Parameters
#define LPIT0_CHANNEL 0
#define LPIT_FREQUENCY 1000

// PWM Parameters
#define PWM_HIGH_FREQ 60000
#define PWM_LOW_FREQ 20000

#define CMAX(frequency) ((PWM_CLOCK_FREQ/frequency - 1))

// Lookup Tables
#define LUT_SIZE 10               // Ten samples/period
static float sineTable[LUT_SIZE];
static float dutyTable[LUT_SIZE];
static uint32_t cthresTable[LUT_SIZE];
// uint32_t cthresTable[LUT_SIZE]={83, 122, 146, 146, 122, 83, 43, 19, 19, 43};
// static:        722ns
// static const:  760ns
// const:         760ns
// none:          722ns

#define LUT_START &cthresTable[0]
#define LUT_END &cthresTable[LUT_SIZE-1]

#define PI 3.14159f

/* GPIO Parameters */
#define LED_BASE 3
#define LED_PIN 0
#define LED_HIGH 1
#define LED_LOW 0

#define LED_PDOR_REG ( *(volatile uint32_t*)(0x400ff0c0)  )
#define PWM_CNV_REG  ( *(volatile uint32_t*)(0x40038010)  )

void compute_sin_isr()
{
  int i;
	float theta;
  float Ts = 2*PI/LUT_SIZE; /* PIT frequency is set to 1 kHz sample rate */

	for(i=0; i<LUT_SIZE; i++){
	  theta = Ts*i;
	  sineTable[i]=sinf(theta);
	  dutyTable[i]=0.5f + (0.4f * sineTable[i]);
	  cthresTable[i] = dutyTable[i]*(CMAX(PWM_HIGH_FREQ)+1);
	}
}

void IsrA(void)
{
	uint32_t iAnalog = 0;
	float dutyCycle = 0;
	int frequency = 0;

	/* Turn on LED */
	writeGPIO(3, 0, 1);

	/* Read sine value */
	iAnalog = read_ADC0_single(0);

	/* Calculate PWM duty cycle */
	if (readGPIO(4, 7) != 0)
	{
		dutyCycle = ((iAnalog / 5000.0) * 0.8) + 0.1;
	}
	else
	{
		dutyCycle = ((iAnalog / 5000.0) * 0.2) + 0.4;
	}

	/* Set PWM frequency based on dipswitch */
	if (readGPIO(4, 6) != 0)
	{
		frequency = PWM_HIGH_FREQ;
	}
	else
	{
		frequency = PWM_LOW_FREQ;
	}

	/* Set PWM duty cycle and frequency */
	setPWM(MOTOR_SUBMODULE, MOTOR_CHANNEL, frequency, dutyCycle);

	/* Turn off LED */
	writeGPIO(3, 0, 0);

	/* Clear interrupt flag */
	clearFlagLPIT(LPIT0_CHANNEL);
}

void IsrB(void)
{
	static int i = 0;
	float theta;
	float dutyCycle = 0;

	/* Turn on LED */
	writeGPIO(3, 0, 1);

	/* Calculate and set PWM duty cycle */
	theta = 2 * PI * i / 10;
	i = (i + 1) % 10;
	dutyCycle = 0.5 + 0.4 * sinf(theta);
	setPWM(MOTOR_SUBMODULE, MOTOR_CHANNEL, PWM_HIGH_FREQ, dutyCycle);

	/* Turn off LED */
	writeGPIO(3, 0, 0);

	/* Clear interrupt flag */
	clearFlagLPIT(LPIT0_CHANNEL);
}

// 6.1us
void IsrC(void)
{
	float dutyCycle = 0;
	static int i = 0;

	/* Turn on LED */
	writeGPIO(3, 0, 1);

	/* Calculate and set PWM duty cycle */

	dutyCycle = 0.5 + 0.4 * sineTable[i];
	setPWM(MOTOR_SUBMODULE, MOTOR_CHANNEL, PWM_HIGH_FREQ, dutyCycle);

	if (i < LUT_SIZE - 1)
		i++;
	else
		i = 0;
	/* Turn off LED */
	writeGPIO(3, 0, 0);

	/* Clear interrupt flag */
	clearFlagLPIT(LPIT0_CHANNEL);
}


// 6.1 us unchanged IsrC
// 2.5 us  by precalculating PWM duty cycle lookup table (dutyTable)
// 1640ns by inlining setPWM and using a constant macro for CMAX (removes 1 division)
// 1273ns by precalculating PWM cthres lookup table (cthresTable)
//  760ns by inlining writeGPIO and removing unnecessary memory accesses
//  735ns using pointer arithmetic to access the LUT
//  685ns by only setting the PWM frequency in main() instead of in the ISR.
//  598ns by changing cthresTable from uint16 to uint32
//  522ns using compiler optimization -O1
//  109ns using compiler optimization -O3 (sine wave skips some samples?)
void IsrC_Optimized(void)
{
  static const uint32_t* lut_ptr = LUT_START;

  // Turn on LED
  (gpio_base_ptrs[LED_BASE]->PDOR) = (1<<LED_PIN);
  //LED_PDOR_REG = (1<<LED_PIN);


  // Set PWM frequency and duty cycle
  FTM_MODULE[MOTOR_SUBMODULE]->CONTROLS[MOTOR_CHANNEL].CnV = *lut_ptr;  // Duty cycle
  //PWM_CNV_REG = *lut_ptr;  // Duty cycle


  // Increment LUT pointer
  if (lut_ptr < LUT_END){
    lut_ptr++;
  } else {
    lut_ptr = LUT_START;
  }

  // Turn off LED
  (gpio_base_ptrs[LED_BASE]->PDOR) = 0;
  //LED_PDOR_REG = 0;

  /* Clear interrupt flag */
  clearFlagLPIT(LPIT0_CHANNEL);
}


void main()
{
	initEECS461();

	//volatile uint32_t* pdor = &(gpio_base_ptrs[LED_BASE]->PDOR);
	//volatile uint32_t* cnv  = &(FTM_MODULE[MOTOR_SUBMODULE]->CONTROLS[MOTOR_CHANNEL].CnV);

  // SIN lookup table
  compute_sin_isr();

	//PWM
  initPWMPCRs();
  initPWM(MOTOR_SUBMODULE, MOTOR_CHANNEL, MOTOR_FREQUENCY, 0.5);
  initPWM(FILTER_SUBMODULE, FILTER_CHANNEL, FILTER_FREQUENCY, FILTER_DUTY_CYCLE);
  FTM_MODULE[MOTOR_SUBMODULE]->MOD = CMAX(PWM_HIGH_FREQ);               // Frequency

  // GPIO
  initGPDI(4, 6); // DIP 1  (pwm frequency)
  initGPDI(4, 7); // DIP 2  (duty cycle range)
  initGPDO(3, 0); // LED

  //ADC
  init_ADC0_single();

  // Interrupts
  enableLPIT();
	initLPIT(LPIT0_CHANNEL, LPIT_FREQUENCY, &IsrC_Optimized, 0xC);

	while (1){}
}
