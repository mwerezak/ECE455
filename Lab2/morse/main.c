#include <lpc17xx.h>
#include "../mcb1700_ece/glcd.h"

#include "../common/led.h"
#include "../common/pushbutton.h"
#include "../common/morsereader.h"

int main()
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);

	LED_Init();
	Pushbutton_Init();
	Timer_Init();

	MorseReader_Init();
	MorseReader_TestRun();

	return 0;
}
