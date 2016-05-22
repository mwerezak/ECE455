#include <lpc17xx.h>
#include "glcd.h"
#include "lab1.h"

int main(void)
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);

	runSoftwareTimer();

	return 0;
}
