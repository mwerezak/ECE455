#include "timer.h"

//*** Timer

void Timer_Init(void)
{
	Timer_Reset();
	LPC_TIM0->PR = TIM0_FREQUENCY/1000; //tick milliseconds
	LPC_TIM0->MCR &= ~(0x03); //ignore match register
}

void Timer_Start(void)
{
	LPC_TIM0->TCR = 0x01; //timer enable
}

void Timer_Reset(void)
{
	LPC_TIM0->TCR = 0x02; // reset timer
}

int Timer_GetTicks(void)
{
	return LPC_TIM0->TC;
}
