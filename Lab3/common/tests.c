//*** Tests

#include <lpc17xx.h>
#include "../mcb1700_ece/glcd.h"

#include "../common/led.h"
#include "../common/timer.h"
#include "../common/pushbutton.h"
#include "../common/debounce.h"
#include "../common/debouncetest.h"

#include <stdio.h>

#ifdef DEBOUNCE_TEST
int main(void)
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);

	LED_Init();
	Pushbutton_Init();
	Timer_Init(LPC_TIM0);
	Timer_EnableInterrupts(0x1);

	Debounce_Init(LPC_TIM0, &DebounceTest_Handler);
	DebounceTest_Run();

	while(1);

}
#endif

#ifdef TIMER_TEST
int _stop = 0;
int main(void)
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);

	LED_Init();
	Pushbutton_Init();
	Timer_Init(LPC_TIM0);
	Timer_Init(LPC_TIM1);
	Timer_EnableInterrupts(0x3);

	LED_On(1);

	Timer_SetTimeout(LPC_TIM0, 3000);
	Timer_SetInterval(LPC_TIM1, 500);
	do
	{
		unsigned char strbuf[20];
		snprintf((char*)strbuf, 20, "%d", Timer_GetTicks(LPC_TIM0));
		GLCD_DisplayString(1, 0, 1, strbuf);
	}
	while(!_stop);

	GLCD_DisplayString(2, 0, 1, "DONE");

	while(1);

}

void EINT3_IRQHandler(void)
{
	Pushbutton_ClearInterrupt();

	LED_On(0);
}

int led_toggle = 0;

void TIMER0_IRQHandler(void)
{
	Timer_ClearInterrupt(LPC_TIM0);
	LED_On(2);
	_stop = 1;
}

void TIMER1_IRQHandler(void)
{
	Timer_ClearInterrupt(LPC_TIM1);
	led_toggle = !led_toggle;
	LED_Set(3, led_toggle);
}
#endif
