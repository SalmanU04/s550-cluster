/*
 * CAN S550 TX + RX
 * Written for TI Tiva TM4C123GH6PM
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

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
#include "driverlib/timer.h"

#include "utils/uartstdio.c"

#include "s550_cluster.h"
#include "s550_uart.h"
#include "s550_timer.h"



// Global flags
volatile bool rxFlag = false;
volatile bool errFlag = false;

unsigned char key_input = 0;

// Clock speed
unsigned int sysClock = 80000000;

// TX message (8-byte CAN message to be sent)
uint8_t txData[8];

// RX message buffer
uint8_t rxData[8];

// Delay function
void delay(unsigned int milliseconds) {
    SysCtlDelay((sysClock / 3) * (milliseconds / 1000.0f));
}

// CAN Interrupt Handler
void CANIntHandler(void) {
    uint32_t status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);


    if(status == CAN_INT_INTID_STATUS) {
        CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
        errFlag = true;
    }
    else if(status == 1) {  // RX Msg Obj 1
        CANIntClear(CAN0_BASE, 1);
        rxFlag = true;
        errFlag = false;
    }
    else if(status == 2) {  // TX Msg Obj 2
        CANIntClear(CAN0_BASE, 2);
        errFlag = false;
    }
    else {
        UARTprintf("Unexpected CAN interrupt\n");
    }
}

int main(void) {

    // Set system clock to 80 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    sysClock = SysCtlClockGet();

    // UART0 init (PA0/PA1)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, sysClock);

    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);
    UARTIntRegister(UART0_BASE, UART0Handler);
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    UARTEnable(UART0_BASE);

    // CAN0 init (PE4=RX, PE5=TX)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    GPIOPinConfigure(GPIO_PE4_CAN0RX);
    GPIOPinConfigure(GPIO_PE5_CAN0TX);
    GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    CANInit(CAN0_BASE);
    CANBitRateSet(CAN0_BASE, sysClock, 500000);
    CANIntRegister(CAN0_BASE, CANIntHandler);
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
    IntEnable(INT_CAN0);
    CANEnable(CAN0_BASE);

    // Timer0 init ()
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    uint32_t timerPer = sysClock * 0.05;
    TimerLoadSet(TIMER0_BASE, TIMER_A, timerPer-1);
    TimerIntRegister(TIMER0_BASE, TIMER_A, TIMER0Handler);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER0A);
    TimerEnable(TIMER0_BASE, TIMER_A);





    // Configure RX Message Object (1)
    rxMsg.ui32MsgID = 0;
    rxMsg.ui32MsgIDMask = 0;
    rxMsg.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    rxMsg.ui32MsgLen = 8;
    rxMsg.pui8MsgData = rxData;
    CANMessageSet(CAN0_BASE, 1, &rxMsg, MSG_OBJ_TYPE_RX);

    // Configure TX Message Object (2)


    UARTprintf("CAN TX + RX Initialized\n");


    while (1) {
        startup();

        // Speed should be inputed 100 times more than actual, RPM should be 2 times more than actual
        if (timerFlag == 1){
            setSpeedRPM(speed, RPM);
            timerFlag = 0;
        }

        interpret_key(key_input);
        key_input = 0;

        // Check for errors
        if (errFlag) {
            UARTprintf("\nCAN Bus Error Detected!\n");
            errFlag = false;
        }



        delay(50); // 1 second delay

        // If received a message
        //if (rxFlag) {
        //    rxFlag = false;
        //
        //    // Reassign buffer (just in case)
        //    rxMsg.pui8MsgData = rxData;
        //    CANMessageGet(CAN0_BASE, 1, &rxMsg, false);
        //
        //    UARTprintf("\nReceived Message ID: 0x%X: ", rxMsg.ui32MsgID);
        //  int i;
        //  for (i = 0; i < rxMsg.ui32MsgLen; i++) {
        //      UARTprintf("0x%02X ", rxData[i]);
        //  }
        //}

    }
}
