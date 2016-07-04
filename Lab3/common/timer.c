#include "timer.h"

//*** Timer

void Timer_Init(LPC_TIMER *timer)
{
	Timer_Reset(timer);
	timer->PR = TIM0_FREQUENCY/1000; //tick milliseconds
	timer->MCR &= ~(0x03); //ignore match register
}

void Timer_Start(LPC_TIMER *timer)
{
	timer->TCR = 0x01; //timer enable
}

void Timer_Reset(LPC_TIMER *timer)
{
	timer->TCR = 0x02; // reset timer
}

int Timer_GetTicks(LPC_TIMER *timer)
{
	return timer->TC;
}
