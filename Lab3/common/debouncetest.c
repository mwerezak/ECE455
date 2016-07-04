#include "debouncetest.h"

#include "../mcb1700_ece/glcd.h"
#include "debounce.h"

static int _input_recieved;
static int _last_input;

void DebounceTest_Handler(int input_event)
{
	_input_recieved = 1;
	_last_input = input_event;
}

void DebounceTest_Run(void)
{
	_input_recieved = 0;

	while(1)
	{
		if(_input_recieved)
		{
			_input_recieved = 0;

			if(_last_input == INPUT_PRESSED)
			{
				GLCD_DisplayString(0, 0, 1, "PRESSED");
			}
			else if(_last_input == INPUT_RAISED)
			{
				GLCD_DisplayString(0, 0, 1, "RAISED ");
			}
			else
			{
				GLCD_DisplayString(0, 0, 1, "       ");
			}
		}
	}
}
