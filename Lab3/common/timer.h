#ifndef TIMER_H
#define TIMER_C

#include <lpc17xx.h>

#define TIMER_FREQUENCY (SystemFrequency/4)

typedef LPC_TIM_TypeDef LPC_TIMER;

//*** Timer

void Timer_Init(LPC_TIMER *timer);
void Timer_EnableInterrupts(int enable_mask);

void Timer_Start(LPC_TIMER *timer);
void Timer_SetTimeout(LPC_TIMER *timer, int timeout);
void Timer_SetInterval(LPC_TIMER *timer, int interval);

void Timer_Reset(LPC_TIMER *timer);
int Timer_GetTicks(LPC_TIMER *timer);

void Timer_ClearInterrupt(LPC_TIMER *timer);

#endif
