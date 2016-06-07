#include <lpc17xx.h>
#include <stdio.h>
#include "mcb1700_ece/glcd.h"

/*
	HARDWARE TIMER RESULTS
		Hardware interrupt Clock: 10:00 (600 s)
		Stopwatch: 9:59 (599 s)
	
*/
int elapsed_secs = 0;
int updateClock = 1;

void displayFormattedTime(unsigned int elapsed_secs)
{
	unsigned int elapsed_mins = elapsed_secs/60;

	const int str_sz = 6;
	unsigned char clock_str[str_sz];
	snprintf((char*)clock_str, str_sz, "%02u:%02u", elapsed_mins % 60, elapsed_secs % 60);

	GLCD_DisplayString(0, 0, 1, clock_str);
	GLCD_DisplayString(1, 0, 1, "HARDWARE");
}

void runHardwareTimer()
{
	LPC_TIM0->TCR = 0x02; // reset timer
	LPC_TIM0->TCR = 0x01; // enable timer
	LPC_TIM0->MR0 = SystemCoreClock/4; // match value (Pclk = Cclk/4 according to PCLKSEL0)
	LPC_TIM0->MCR |= 0x03; // on match, generate interrupt and reset
	NVIC_EnableIRQ(TIMER0_IRQn); // allow interrupts from the timer

	while(1)
	{
		if(updateClock) 
		{
			displayFormattedTime(elapsed_secs);
			updateClock = 0;

			if(elapsed_secs >= 60*10)
			{
				LPC_TIM0->TCR = 0x00; // disable timer
				return;
			}
		}
	}
}

void TIMER0_IRQHandler(void)
{
	LPC_TIM0->IR |= 0x01;

	elapsed_secs++;
	updateClock = 1;
}

int main(void)
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);

	runHardwareTimer();

	return 0;
}
