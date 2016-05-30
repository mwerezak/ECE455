#include <lpc17xx.h>
#include "../mcb1700_ece/glcd.h"

int main()
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);
	GLCD_DisplayString(0, 0, 1, "Hello, world!");
	return 0;
}
