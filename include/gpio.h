/*
  gpio.c
  University of Michigan
  EECS 461, Embedded Control Systems
  General-Purpose Input/Output module
  Read Chapter 12&13 in S32K144 User's Manual
 */

#ifndef __GPIO_H__
#define __GPIO_H__

#include "S32K144.h"

extern const uint8_t LED[16];
extern const uint8_t DIP[16];
extern const uint8_t LED_BASE[16];
extern const uint8_t DIP_BASE[16];

extern GPIO_Type * gpio_base_ptrs[5];
extern PORT_Type * port_base_ptrs[5];

/*=============================================================================
  Section 1: GPIO initialization
  The module provides dedicated general-purpose pads
  that can be configured as either inputs or outputs.
=============================================================================*/

void initGPDI(const uint8_t base, const uint8_t pin);

void initGPDO(const uint8_t base, const uint8_t pin);


/*=============================================================================
  Section 2: GPIO utilization
=============================================================================*/

uint8_t readGPIO(const uint8_t base, const uint8_t pin);

void writeGPIO(const uint8_t base, const uint8_t pin, const uint8_t val);

#endif
