#include "s550_timer.h"
#include "s550_cluster.h"

int timerFlag = 0;

void TIMER0Handler(){
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    timerFlag = 1;
}
