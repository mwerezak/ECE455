#include "timer.h"

#include <lpc17xx.h>
//*** Timer

void Timer_Init(LPC_TIMER *timer)
{
	Timer_Reset(timer);
	timer->PR = TIM0_FREQUENCY/1000; //tick milliseconds

	NVIC_EnableIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER1_IRQn);
}

void Timer_Start(LPC_TIMER *timer)
{
	timer->TCR = 0x01; //timer enable
}

void Timer_Reset(LPC_TIMER *timer)
{
	timer->TCR = 0x02; // reset timer
	timer->MCR &= ~(0x07); //clear all match control bits
}

int Timer_GetTicks(LPC_TIMER *timer)
{
	return timer->TC;
}

void Timer_SetTimeout(LPC_TIMER *timer, int timeout)
{
	Timer_Reset(timer);

	timer->MCR |= 0x5; //On match, stop timer and interrupt
	timer->MR0 = timeout;

	Timer_Start(timer);
}

void Timer_SetInterval(LPC_TIMER *timer, int interval)
{
	Timer_Reset(timer);

	timer->MCR |= 0x3; //On match, reset timer and interrupt
	timer->MR0 = interval;

	Timer_Start(timer);
}

void Timer_ClearInterrupt(LPC_TIMER *timer)
{
	timer->IR |= 0x1; //we only ever use Match Channel 0.
}
