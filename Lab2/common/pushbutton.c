/*
	On initial press, starts a timer and then for every INT0 event afterwards
	records and the time since the initial press.
*/

#include "pushbutton.h"

//*** Pushbutton

void Pushbutton_Init(void)
{
	LPC_PINCON->PINSEL4 &= ~(3<<20); // P2.10 is GPIO
	LPC_GPIO2->FIODIR &= ~(1<<10); // P2.10 is input

	//which edge doesn't matter really, since it's going to bounce anyways
	LPC_GPIOINT->IO2IntEnR |= (1 << 10); // rising edge of P2.10
	LPC_GPIOINT->IO2IntEnF |= (1 << 10); // falling edge of P2.10
	NVIC_EnableIRQ(EINT3_IRQn);
}

void Pushbutton_ClearInterrupt(void)
{
	LPC_GPIOINT->IO2IntClr |= 1 << 10; // clear interrupt condition
}

//0 - raised, 1 - depressed
int Pushbutton_ReadValue(void)
{
	return ~(LPC_GPIO2->FIOPIN >> 10) & 0x01;
}

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

