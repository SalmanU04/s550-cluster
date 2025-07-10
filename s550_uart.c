#include "s550_uart.h"
#include "s550_cluster.h"
extern unsigned char key_input;

#define DOWN 0x01
#define LEFT 0x02
#define RIGHT 0x04
#define UP 0x08
#define ENTER 0x10

#define TIME 0.05
#define ACCELERATOR 21.46
#define RPM_CHANGE 200
float speed = 0;        // Global Speed int
float RPM = 800;        // Base RPM


void increaseSpeed(){
    RPM += RPM_CHANGE * 0.5;
    speed += ACCELERATOR * 0.1;
}

void decreaseSpeed(){

    speed -= ACCELERATOR * 0.2;
    RPM -= 100;
    speed = speed < 0 ? 0 : speed;
    RPM = RPM < 800 ? 800 : RPM;
}

void idleSpeed(){
    if (speed > 0){
        speed -= 0.5;
        if (speed < 0){
            speed = 0;
        }
    }
    if (RPM > 800){
        RPM -= 100;
        if (RPM < 800){
            RPM = 800;
        }
    }

}

void UART0Handler(){
    uint32_t status = UARTIntStatus(UART0_BASE, true);
       UARTIntClear(UART0_BASE, status);

       if ((status & UART_INT_RX) || (status & UART_INT_RT)) {
           while (UARTCharsAvail(UART0_BASE)) {
               key_input = UARTCharGet(UART0_BASE);
           }
       }
}

void interpret_key(unsigned char key_input){
    uint8_t wheelData[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 0x81 - Controls wheel buttons
    switch (key_input) {
        case 'W':
            wheelData[0] = UP;
            break;
        case 'A':
            wheelData[0] = LEFT;
            break;
        case 'S':
            wheelData[0] = DOWN;
            break;
        case 'D':
            wheelData[0] = RIGHT;
            break;
        case 0x0D:      // Enter
            wheelData[0] = ENTER;
            break;
        case 'w':
            increaseSpeed();
            break;
        case 's':
            decreaseSpeed();
            break;
        default:
            idleSpeed();
            break;
    }
    sendMsg(0x81, wheelData);
}
