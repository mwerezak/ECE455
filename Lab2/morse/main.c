#include <lpc17xx.h>
#include "../mcb1700_ece/glcd.h"

#include "../common/led.h"
#include "../common/pushbutton.h"

int main()
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);

	DebounceTest_Init();
	DebounceTest_Run();

	return 0;
}
