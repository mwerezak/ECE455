#include "morsereader.h"

#include "timer.h"
#include "pushbutton.h"
#include "led.h"
#include "../mcb1700_ece/glcd.h"
#include <stdio.h>

static int _debouncing;

static int _last_input;
static int _input_recieved;

void MorseReader_Init(void)
{
	_debouncing = DEBOUNCE_READY;

	_last_input = INPUT_NEW;
	_input_recieved = 0;

}

static void MorseReader_UpdateLED(void)
{
	LED_Set(0, _last_input == INPUT_DOT);
	LED_Set(1, _last_input == INPUT_DASH);
}

static void MorseReader_HandlePushbutton(void)
{
	int timer_ticks = Timer_GetTicks(LPC_TIM0);
	int pb_pressed = Pushbutton_ReadValue();

	if(_debouncing == DEBOUNCE_READY)
	{
		if(pb_pressed)
		{
			_debouncing = DEBOUNCE_PRESSED;
			Timer_Start(LPC_TIM0);

			MorseReader_RegisterInput(INPUT_NEW);
		}
	}
	else if(_debouncing == DEBOUNCE_PRESSED)
	{
		if(timer_ticks > DEBOUNCE_DELAY && !pb_pressed)
		{
			MorseReader_RegisterInput(timer_ticks < DASH_DELAY? INPUT_DOT : INPUT_DASH);
			_debouncing = DEBOUNCE_READY;
			Timer_Reset(LPC_TIM0);
		}
	}
}

static void MorseReader_RegisterInput(int input_type)
{
	_input_recieved = 1;
	_last_input = input_type;
	MorseReader_UpdateLED();
}

void MorseReader_TestRun(void)
{
	while(1)
	{
		if(_input_recieved)
		{
			_input_recieved = 0;

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

//Run the FSM
#include "../fsm/fsm_runner.h"

void MorseReader_FSMRun(void)
{
	unsigned char strbuf[20];
	FSMRunner runner;
	FSMRunner_Init(&runner);

	while(1)
	{
		if(_input_recieved)
		{
			_input_recieved = 0;

			if(_last_input == INPUT_DOT)
			{
				GLCD_DisplayString(0, 0, 1, "DOT ");
				if(FSMRunner_ApplyInput(&runner, FSM_DOT))
				{
					FSMRunner_Reset(&runner); //If there was no state to transition to (invalid sequence), reset the FSMRunner
				}
			}
			else if(_last_input == INPUT_DASH)
			{
				GLCD_DisplayString(0, 0, 1, "DASH");
				if(FSMRunner_ApplyInput(&runner, FSM_DASH))
				{
					FSMRunner_Reset(&runner); //If there was no state to transition to (invalid sequence), reset the FSMRunner
				}
			}
			else
			{
				GLCD_ClearLn(0, 1);
			}

			if(runner.has_output)
			{
				GLCD_DisplayString(1, 0, 1, (unsigned char*)runner.cur_output);
			}
			else
			{
				GLCD_ClearLn(1, 1);
			}

			snprintf((char*)strbuf, 20, "State: %u", runner.cur_state->state_id);
			GLCD_DisplayString(5, 0, 1, strbuf);
		}
	}
}
