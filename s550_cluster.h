/*
 * s550_cluster.h
 *
 *  Created on: Jul 1, 2025
 *      Author: salmanumar
 */

#ifndef S550_CLUSTER_H_
#define S550_CLUSTER_H_
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "driverlib/can.h"


extern void sendMsg();
extern void startup();
extern void setSpeedRPM(uint16_t speed, uint16_t rpm);

extern tCANMsgObject rxMsg, txMsg;



#endif /* S550_CLUSTER_H_ */
