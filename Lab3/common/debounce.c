#include "debounce.h"

#include "pushbutton.h"

static int _debouncing;

static LPC_TIMER *_timer;
static DebouncedInputHandler _handler;

void Debounce_Init(LPC_TIMER *debounce_timer, DebouncedInputHandler input_handler)
{
	_debouncing = DEBOUNCE_READY;
	_timer = debounce_timer;
	_handler = input_handler;
}

static void Debounce_HandlePushbutton(void)
{
	int timer_ticks = Timer_GetTicks(_timer);
	int pb_pressed = Pushbutton_ReadValue();

	if(_debouncing == DEBOUNCE_READY)
	{
		if(pb_pressed)
		{
			_debouncing = DEBOUNCE_PRESSED;
			Timer_Start(_timer);

			_handler(INPUT_PRESSED);
		}
	}
	else if(_debouncing == DEBOUNCE_PRESSED)
	{
		if(timer_ticks > DEBOUNCE_DELAY && !pb_pressed)
		{
			_handler(INPUT_RAISED);

			_debouncing = DEBOUNCE_READY;
			Timer_Reset(_timer);
		}
	}
}

//Ensures this handler is only defined if debounce.h is included somewhere
#ifdef DEBOUNCE_ENABLE
void EINT3_IRQHandler(void)
{
	Pushbutton_ClearInterrupt();
	Debounce_HandlePushbutton();
}
#endif
