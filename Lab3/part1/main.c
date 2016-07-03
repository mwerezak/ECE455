#include <lpc17xx.h>
#include "../mcb1700_ece/glcd.h"

int main()
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);

	return 0;
}
