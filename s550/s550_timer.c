#include "s550_timer.h"
#include "s550_cluster.h"
#include "s550_uart.h"

#include <stdio.h>
#include <math.h>

#define PI 3.14

int timer0Flag = 1;
int speedType = 0;

#define PEAK_SPEED 96.6       // 60 in mph
#define PEAK_RPM 3000         // max RPM
#define CYCLE_TIME 9          // seconds for full up-down
#define TIMER_INTERVAL 0.05    // 100 ms
#define TOTAL_TICKS 900       // 90 seconds / 0.1 = 900 ticks


int counter = 0;
void TIMER0Handler(){
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    timer0Flag = 1;

    // Sine Wave
    counter++;

    if (counter > TOTAL_TICKS) {
        counter = 0; // reset after 90 seconds
    }

    double elapsed_seconds = (counter % (int)(CYCLE_TIME / TIMER_INTERVAL)) * TIMER_INTERVAL;
    double spd = (PEAK_SPEED / 2.0) * (1.0 - cos((2.0 * PI / CYCLE_TIME) * elapsed_seconds));

    double rpm = (PEAK_RPM / 2.0) * (1.0 - cos((2.0 * PI / CYCLE_TIME) * elapsed_seconds));


    speed = spd;
    RPM = rpm;
}

