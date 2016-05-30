#include "pushbutton.h"

#include <lpc17xx.h>

//TODO
#define PB_EVENT_NONE 0x0
#define PB_EVENT_RISE 0x1
#define PB_EVENT_FALL 0x2

static int pb_event = PB_EVENT_NONE;

void Pushbutton_Init(void)
{
	LPC_PINCON->PINSEL4 &= ~(3<<20); // P2.10 is GPIO
	LPC_GPIO2->FIODIR &= ~(1<<10); // P2.10 is input

	//which edge doesn't matter really, since it's going to bounce to shit anyways
	LPC_GPIOINT->IO2IntEnR |= (1 << 10); // rising edge of P2.10
	NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler(void)
{
	LPC_GPIOINT->IO2IntClr |= 1 << 10; // clear interrupt condition

	pb_event |= 1; //TODO debounce and set correct events
}

//TODO actual usable interface
//synchronous tight polling for falling edge
void Pushbutton_WaitForEvent(void) //TODO timeout
{
	while(!pb_event);
}
