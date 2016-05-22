#include <lpc17xx.h>
#include <stdio.h>
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

/*
	RESULTS
	* 1st run: calibration = 1.0
		Software Delay Clock: 10:00 (600 s)
		Stopwatch: 7:07 (427 s)				   

	* 2nd run: calibration = 1.40
		Software Delay Clock: 10:00	(600 s)
		Stopwatch: 9:45	(585 s)
	
*/
#define SOFTWARE_DELAY_CALIBRATION 1.40
void software_delay(unsigned long delay_ms)
{
	//SystemFrequency is cycles per second
	//cycles per ms is SystemFrequency/1000
	//cycles per N ms is N*SystemFrequency/1000
	const double numiter = SystemFrequency*delay_ms/1000*SOFTWARE_DELAY_CALIBRATION;
	int i;

	__disable_irq(); //Disable interrupts
	for(i = 0; i < numiter; i++); //note: second for loop is in runTimer()
	__enable_irq(); //Enable interrupts
}

void displayFormattedTime(unsigned int elapsed_secs)
{
	unsigned int elapsed_mins = elapsed_secs/60;

	const int str_sz = 6;
	unsigned char clock_str[str_sz];
	snprintf((char*)clock_str, str_sz, "%02u:%02u", elapsed_mins % 60, elapsed_secs % 60);

	GLCD_DisplayString(0, 0, 1, clock_str);
}

void runSoftwareTimer()
{
	int elapsed_sec = 0; //elapsed seconds

	do
	{
		displayFormattedTime(elapsed_sec);
		software_delay(1000);
		elapsed_sec++;
	}
	while(elapsed_sec <= 60*10);
}

int main(void)
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);

	runSoftwareTimer();

	return 0;
}
