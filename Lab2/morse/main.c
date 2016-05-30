#include <lpc17xx.h>
#include "../mcb1700_ece/glcd.h"

#include "../io/led.h"

int main()
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);
	GLCD_DisplayString(0, 0, 1, "Pushbutton Test");

	//GLCD_DisplayString(0, 1, 1, "Falling Edge!");

	LED_Init();
	LED_Set(1);
	LED_Set(3);
	LED_Set(5);
	LED_Set(7);

	return 0;
}
