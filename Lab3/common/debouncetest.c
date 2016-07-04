//*** DebounceTest

#include "debouncetest.h"

#include "pushbutton.h"
#include "../mcb1700_ece/glcd.h"
#include "timer.h"
#include <stdio.h>

static int _debouncing = 0;
static int _last_edge_time = 0;
static int _last_edge_val = 0;
static int _refresh_display = 0;

void DebounceTest_Init(void)
{
	Pushbutton_Init();
	Timer_Init(LPC_TIM0);
	DebounceTest_Reset();
}

void DebounceTest_Reset(void)
{
	_debouncing = 0;
	_refresh_display = 1;
	Timer_Reset(LPC_TIM0);
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

#ifdef DEBOUNCETEST_ENABLE

static void DebounceTest_HandleEdge(int edgeval)
{
	if(_debouncing)
	{
		_last_edge_time = Timer_GetTicks(LPC_TIM0);
	}
	else
	{
		_last_edge_time = 0;
		_debouncing = 1;
		Timer_Start(LPC_TIM0);
	}
	_last_edge_val = edgeval;
	_refresh_display = 1;
}

void EINT3_IRQHandler(void)
{
	Pushbutton_ClearInterrupt();
	DebounceTest_HandleEdge( Pushbutton_ReadValue() );
}

#endif
