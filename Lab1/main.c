#include <lpc17xx.h>
#include "glcd.h"

/*
	LAB1 TODO:

-	Implement a delay ms function using nested for loops.
	DISABLE INTERRUPTS
-	mm:ss clock on the LCD that runs for 10 minutes,
	observe discrepencies with wall clock
-	implement similar clock using hardware timer.
	Compare

*/

void software_delay(unsigned long delay_ms)
{
	//SystemFrequency is cycles per second
	//cycles per ms is SystemFrequency/1000
	//cycles per N is N*SystemFrequency/1000
	const unsigned long numiter = (SystemFrequency/1000)*delay_ms;
	int i;

	__disable_irq(); //Disable interrupts
	for(i = 0; i < numiter; i++);
	__enable_irq(); //Enable interrupts
}

/*
void displayFormattedTime(unsigned long elapsed_ms)
{
	unsigned int elapsed_secs = elapsed_ms/1000;
	unsigned int elapsed_mins = elapsed_ms/(60*1000);

	char clock_str[5];
	snprintf(clock_str, 5, "%02u:%02u", elapsed_mins % 60, elapsed_secs % 60);

	GLCD_DisplayString(0, 0, 1, clock_str);
}
*/

int main(void)
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);
	GLCD_DisplayString(0, 0, 1, "Foo");

	software_delay(1*1000);

	GLCD_DisplayString(0, 3, 1, "bar");


	return 0;
}
