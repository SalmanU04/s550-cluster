/*
 * s550_timer.h
 *
 *  Created on: Jul 8, 2025
 *      Author: salmanumar
 */

#ifndef S550_TIMER_H_
#define S550_TIMER_H_

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"

extern void TIMER0Handler();

extern int timerFlag; // Timer Flag for updating speed

#endif /* S550_TIMER_H_ */
