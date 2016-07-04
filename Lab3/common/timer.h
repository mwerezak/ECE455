#ifndef TIMER_H
#define TIMER_C

#include <lpc17xx.h>

#define TIM0_FREQUENCY (SystemFrequency/4)

typedef LPC_TIM_TypeDef LPC_TIMER;

//*** Timer

void Timer_Init(LPC_TIMER *timer);
void Timer_Start(LPC_TIMER *timer);
void Timer_Reset(LPC_TIMER *timer);
int Timer_GetTicks(LPC_TIMER *timer);

#endif
