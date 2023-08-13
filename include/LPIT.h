/*
  LPIT.h
  University of Michigan
  EECS 461, Embedded Control Systems
  Low Power Interrupt Timer (LPIT)
  Read Chapter 30 in S32K144 User's Manual
 */

#ifndef LPIT_H_
#define LPIT_H_

#include "S32K144.h"
#include "eecs461.h"
#include "interrupt_manager.h"

#define LPIT0_CHANNEL 0
#define LPIT_FREQUENCY 1000
#define LPIT_CLK_FREQ SPLLDIV2_CLK_FREQ


void enableLPIT();
void initLPIT(const uint8_t channel, const uint32_t frequency, const isr_t handler, const uint32_t priority);
void clearFlagLPIT(const uint8_t channel);

#endif /* LPIT_H_ */
