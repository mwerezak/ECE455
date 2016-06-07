/*
	On initial press, starts a timer and then for every INT0 event afterwards
	records and the time since the initial press.
*/

#include "pushbutton.h"
#include "../mcb1700_ece/glcd.h"
#include <stdio.h>

static int _debouncing = 0;
static int _last_edge_time = 0;
static int _last_edge_val = 0;
static int _refresh_display = 0;

//*** DebounceTest

void DebounceTest_Init(void)
{
	Pushbutton_Init();
	Timer_Init();
	DebounceTest_Reset();
}

void DebounceTest_Reset(void)
{
	_debouncing = 0;
	_refresh_display = 1;
	Timer_Reset();
}

static void DebounceTest_HandleEdge(int edgeval)
{
	if(_debouncing)
	{
		_last_edge_time = Timer_GetTicks();
	}
	else
	{
		_last_edge_time = 0;
		_debouncing = 1;
		Timer_Start();
	}
	_last_edge_val = edgeval;
	_refresh_display = 1;
}

void DebounceTest_Run(void)
{
	while(1)
	{
		if(!_refresh_display) continue;
		_refresh_display = 0;

		if(!_debouncing)
		{
			GLCD_DisplayString(0, 0, 1, "READY");
		}
		else
		{
			DebounceTest_PrintInfo();
		}
	}
}

static void DebounceTest_PrintInfo()
{
	char strbuf[20];

	snprintf(strbuf, 20, "PRESSED - %d", _last_edge_val);
	GLCD_DisplayString(0, 0, 1, (unsigned char*)strbuf);

	snprintf(strbuf, 20, "%d", _last_edge_time);
	GLCD_DisplayString(1, 0, 1, (unsigned char*)strbuf);
}

void EINT3_IRQHandler(void)
{
	Pushbutton_ClearInterrupt();

	DebounceTest_HandleEdge( Pushbutton_ReadValue() );
}

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

void __inline Pushbutton_ClearInterrupt(void)
{
	LPC_GPIOINT->IO2IntClr |= 1 << 10; // clear interrupt condition
}

//0 - raised, 1 - depressed
int __inline Pushbutton_ReadValue(void)
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

void __inline Timer_Start(void)
{
	LPC_TIM0->TCR = 0x01; //timer enable
}

void __inline Timer_Reset(void)
{
	LPC_TIM0->TCR = 0x02; // reset timer
}

int __inline Timer_GetTicks(void)
{
	return LPC_TIM0->TC;
}

