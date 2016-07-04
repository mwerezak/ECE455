//#include "led.h"

#include <lpc17xx.h>
#include <rtl.h>

#define NUM_LED 8
const U8 led_pos[NUM_LED] = { 28, 29, 31, 2, 3, 4, 5, 6 };

void LED_Init(void)
{
	LPC_GPIO1->FIODIR |= 0xB0000000; // LEDs on PORT1
	LPC_GPIO2->FIODIR |= 0x0000007C; // LEDs on PORT2
}

void LED_On(int led)
{
	int mask;

	if(led < 0 || led >= NUM_LED) return;

	mask = 1 << led_pos[led];
	if (led < 3) { LPC_GPIO1->FIOSET = mask; }
	else { LPC_GPIO2->FIOSET = mask; }
}

void LED_Clear(int led)
{
	int mask;

	if(led < 0 || led >= NUM_LED) return;

	mask = 1 << led_pos[led];
	if (led < 3) { LPC_GPIO1->FIOCLR = mask; }
	else { LPC_GPIO2->FIOCLR = mask; }
}

void LED_Set(int led, int set)
{
	if(set) { LED_On(led); } else { LED_Clear(led); }
}
