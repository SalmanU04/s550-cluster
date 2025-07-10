/*
 * s550_cluster.c
 *
 *  Created on: Jul 1, 2025
 *      Author: salmanumar
 */

#include "s550_cluster.h"
#include "s550_uart.h"
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"

tCANMsgObject rxMsg, txMsg;

void sendMsg(uint32_t msgID, uint8_t *msgData){
    // Set txMsg
    txMsg.ui32MsgID = msgID;
    txMsg.ui32MsgIDMask = 0;
    txMsg.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    txMsg.ui32MsgLen = 8;
    txMsg.pui8MsgData = msgData;

    // Send the message
    UARTprintf("\nSending to ID 0x%X: ", msgID);
    int i;
    for (i = 0; i < 8; i++) {
        UARTprintf("0x%02X ", msgData[i]);
    }
    CANMessageSet(CAN0_BASE, 2, &txMsg, MSG_OBJ_TYPE_TX);
}



void startup(){
    // Initial state of the cluster, should always reset start as this...
    uint8_t sendOn[8] = {0x40, 0x48, 0x02, 0x0A, 0x10, 0x05, 0x00, 0x02}; //0x3B3 - Controls start up
    sendMsg(0x3B3, sendOn);     // Door Ajar does not work

    uint8_t airbagSeatbelt[8] = {0x10, 0x5F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00}; // 0x4C - Controls Airbag and seatbelt indictators
    sendMsg (0x4C, airbagSeatbelt);     //Works for Airbag

    uint8_t absTraction[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 0x416 - Controls ABS, traction control, and brake system warning
    sendMsg (0x416, absTraction);       // Works for ABS and traction

    uint8_t tirePressure[8] = {0x00, 0xCE, 0x00, 0xCE, 0x00, 0xCE, 0x00, 0xCE}; // 0x3B5 - Tire Pressue, set to a consistent 30 PSI/ 206 kPA
    sendMsg (0x3B5, tirePressure);      // Doesn't work

    uint8_t engineTemp[8] = {0x96, 0xAA, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00}; // 0x156 - Coolant Gauge, Oil Temp Gauge, Overheat Message
    sendMsg (0x156, engineTemp);

    uint8_t fuel[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00}; // 0x318
    sendMsg (0x318, fuel);              // Does not work

    uint8_t doorAjar[8] = {0x4C, 0x48, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00 }; // 0x3C3 - Parking Light, High Beam
    sendMsg (0x3C3, doorAjar);          // Works

    uint8_t engine[8] = {0x72, 0x7F, 0x4B, 0x00, 0x00, 0x1A, 0xED, 0x00}; // 0x167 - Engine state, Engine load, Manifold Abs Pressure
    sendMsg(0x167, engine);

    uint8_t sendNull[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendMsg (0x81, sendNull);
}


void setSpeedRPM(uint16_t speed, uint16_t rpm){
    // b6<<8+b7: Speed, cluster wants multiplied by 100.
    UARTprintf("\nSpeed: ", speed);
    int clusterSpeed = speed * 100;
    uint8_t speedSet[8] = {0x0C, 0x00, 0x00, 0x00, 0x60, 0x00, clusterSpeed>>8, clusterSpeed&0xFF}; // 0x202
    sendMsg(0x202, speedSet);

    // b3<<8+b4: RPM, cluster takes 1/2 of actual RPM
    rpm = rpm/2;
    uint8_t rpmSet[8] = {0x00, 0x00, 0x00, rpm>>8, rpm&0xFF, 0x00, 0x00, 0x00};   // 0x204
    sendMsg(0x204, rpmSet);
}

