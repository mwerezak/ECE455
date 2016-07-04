/*
	Bursty Software Scheduler

	Max Burst Size: 3
	Max Arrival Rate: 10 seconds

	On the LCD, display if we are accepting or ignoring interrupts, and 

	If an interrupt is processed, light LED-28 for 200 ms.

	LPC_TIM0 for timer interval.
	LPC_TIM1 for turning off the LED after 200 ms (so that it is visible)
	LPC_TIM2 for debouncing.
*/

#include <lpc17xx.h>
#include "../mcb1700_ece/glcd.h"

#include "../common/led.h"
#include "../common/timer.h"
#include "../common/pushbutton.h"
#include "../common/debounce.h"
#include "../common/debouncetest.h"

#include <stdio.h>

#define MAX_BURST 3
#define BURST_ARRIVAL_PERIOD 10000
#define LED_LIT_TIME 200

int burst_count = 0;
int processed_count = 0;

void StartAcceptingInterrupts(void)
{
	NVIC_EnableIRQ(EINT3_IRQn);

	GLCD_DisplayString(2, 0, 1, "Ext Interrupt");
	GLCD_DisplayString(3, 0, 1, "ENABLED ");
}

void StartIgnoringInterrupts(void)
{
	NVIC_DisableIRQ(EINT3_IRQn);
	Debounce_Reset();

	GLCD_DisplayString(2, 0, 1, "Ext Interrupt");
	GLCD_DisplayString(3, 0, 1, "DISABLED");
}

void PrintBurstCount(void)
{
	unsigned char linebuf[20];
	snprintf((char*)linebuf, 20, "Burst: %d", burst_count);
	GLCD_DisplayString(5, 0, 1, linebuf);
}

void PrintProcessedCount(void)
{
	unsigned char linebuf[20];
	snprintf((char*)linebuf, 20, "Processed: %d", processed_count);
	GLCD_DisplayString(6, 0, 1, linebuf);
}

void ProcessInterrupt(int pb_event)
{
	LED_On(0);
	Timer_SetTimeout(LPC_TIM1, LED_LIT_TIME);

	burst_count++;
	processed_count++;

	if(burst_count >= MAX_BURST)
	{
		StartIgnoringInterrupts();
	}

	PrintBurstCount();
	PrintProcessedCount();
}

void BurstReset(void)
{
	burst_count = 0;
	StartAcceptingInterrupts();
	PrintBurstCount();
}

void TIMER0_IRQHandler(void)
{
	Timer_ClearInterrupt(LPC_TIM0);

	BurstReset(); //regular reset
}

void TIMER1_IRQHandler(void)
{
	Timer_ClearInterrupt(LPC_TIM1);
	LED_Clear(0);
}

int main(void)
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);

	LED_Init();
	Pushbutton_Init();

	Timer_Init(LPC_TIM0); //Timer Interval
	Timer_Init(LPC_TIM1); //LED Pulse
	Timer_Init(LPC_TIM2); //Debouncing
	Timer_EnableInterrupts(0x3); //debouncing doesn't use timer interrupts, so we only need bit 0 and 1

	Debounce_Init(LPC_TIM2, &ProcessInterrupt);

	GLCD_DisplayString(0, 0, 1, "Bursty Scheduler");
	PrintProcessedCount();

	Timer_SetInterval(LPC_TIM0, BURST_ARRIVAL_PERIOD); //Start the interval timer
	BurstReset();

	while(1);
}



