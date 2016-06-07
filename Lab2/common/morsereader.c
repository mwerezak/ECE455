#include "morsereader.h"

#include "pushbutton.h"
#include "led.h"
#include "../mcb1700_ece/glcd.h"

static int _debouncing;

static int _last_input;
static int _update_display;

void MorseReader_Init(void)
{
	_debouncing = DEBOUNCE_READY;

	_last_input = INPUT_NEW;
	_update_display = 0;

}

static void MorseReader_UpdateLED(void)
{
	LED_Set(0, _last_input == INPUT_DOT);
	LED_Set(1, _last_input == INPUT_DASH);
}

static void MorseReader_HandlePushbutton(void)
{
	int timer_ticks = Timer_GetTicks();
	int pb_pressed = Pushbutton_ReadValue();

	if(_debouncing == DEBOUNCE_READY)
	{
		if(pb_pressed)
		{
			_debouncing = DEBOUNCE_PRESSED;
			Timer_Start();

			MorseReader_RegisterInput(INPUT_NEW);
		}
	}
	else if(_debouncing == DEBOUNCE_PRESSED)
	{
		if(timer_ticks > DEBOUNCE_DELAY && !pb_pressed)
		{
			MorseReader_RegisterInput(timer_ticks < DASH_DELAY? INPUT_DOT : INPUT_DASH);
			_debouncing = DEBOUNCE_READY;
			Timer_Reset();

			/*
			_debouncing = DEBOUNCE_DEPRESSED;
			Timer_Reset();

			MorseReader_RegisterInput(timer_ticks < DASH_DELAY? INPUT_DOT : INPUT_DASH);

			Timer_Start();
			*/
		}
	}
	else if(_debouncing == DEBOUNCE_DEPRESSED)
	{
		if(timer_ticks > DEBOUNCE_DELAY)
		{
			_debouncing = DEBOUNCE_READY;
			Timer_Reset();
		}
	}
}

static void MorseReader_RegisterInput(int input_type)
{
	_update_display = 1;
	_last_input = input_type;
	MorseReader_UpdateLED();
}

void MorseReader_TestRun()
{
	while(1)
	{
		if(_update_display)
		{
			_update_display = 0;

			if(_last_input == INPUT_DOT)
			{
				GLCD_DisplayString(0, 0, 1, "DOT ");
			}
			else if(_last_input == INPUT_DASH)
			{
				GLCD_DisplayString(0, 0, 1, "DASH");
			}
			else
			{
				GLCD_DisplayString(0, 0, 1, "    ");
			}
		}
	}
}

#ifdef MORSEREADER_ENABLE

void EINT3_IRQHandler(void)
{
	Pushbutton_ClearInterrupt();
	MorseReader_HandlePushbutton();
}

#endif
