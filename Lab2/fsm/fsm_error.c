#include "fsm_error.h"

#include "../common/led.h"
#include "../mcb1700_ece/glcd.h"

void FSM_RaiseError(unsigned char* error_message)
{
	LED_On(7);

	GLCD_DisplayString(0, 0, 1, error_message);
}
