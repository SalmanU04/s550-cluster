/*
 * s550_uart.h
 *
 *  Created on: Jul 1, 2025
 *      Author: salmanumar
 */

#ifndef S550_UART_H_
#define S550_UART_H_

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "driverlib/uart.h"
#include "inc/hw_memmap.h"

extern float speed;
extern float RPM;

extern void interpret_key(unsigned char key_input);
extern void UART0Handler();


#endif /* S550_UART_H_ */
