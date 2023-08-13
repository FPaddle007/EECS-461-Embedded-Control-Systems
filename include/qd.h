/*
   qd.h
   University of Michigan
   EECS 461, Embedded Control Systems
   FlexTimer Module (FTM): Quadrature Decode
   Read Chapter 44 in S32K144 User's Manual
 */
#ifndef QD_H_
#define QD_H_

#include "S32K144.h"

/* Functions for Quadrature Decoding */
void initQD();
int32_t updateCounter();
float updateAngle();

#endif /* QD_H_ */
