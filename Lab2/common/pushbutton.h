#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <lpc17xx.h>

#define TIM0_FREQUENCY (SystemCoreClock/4)

//*** Pushbutton

void Pushbutton_Init(void);
void Pushbutton_ClearInterrupt(void);
int Pushbutton_ReadValue(void);

//*** Timer

void Timer_Init(void);
void Timer_Start(void);
void Timer_Reset(void);
int Timer_GetTicks(void);

#endif
