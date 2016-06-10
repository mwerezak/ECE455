#ifndef TIMER_H
#define TIMER_C

#include <lpc17xx.h>

#define TIM0_FREQUENCY (SystemCoreClock/4)

//*** Timer

void Timer_Init(void);
void Timer_Start(void);
void Timer_Reset(void);
int Timer_GetTicks(void);

#endif
