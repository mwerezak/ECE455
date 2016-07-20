
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "edf_tasks.h"

/* Standard include. */
#include <stdio.h>

/* The ITM port is used to direct the printf() output to the serial window in 
the Keil simulator IDE. */
#define mainITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define mainITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define mainDEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define mainTRCENA          0x01000000

/*-----------------------------------------------------------*/

/*
 * The tasks
 */

static void DoNothingTask( void *pvParameters );

/*
 * Redirects the printf() output to the serial window in the Keil simulator
 * IDE.
 */
int fputc( int iChar, FILE *pxNotUsed );

/*-----------------------------------------------------------*/

/* One array position is used for each task created by this demo.  The 
variables in this array are set and cleared by the trace macros within
FreeRTOS, and displayed on the logic analyzer window within the Keil IDE -
the result of which being that the logic analyzer shows which task is
running when. */
unsigned long ulTaskNumber[ configEXPECTED_NO_RUNNING_TASKS ];

/*-----------------------------------------------------------*/

int main(void)
{
	/* Start some tasks */
	//xTaskCreate( DoNothingTask, (char *) "Dummy", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	//xTaskCreate( DoNothingTask, (char *) "Dummy", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
	CreateTask(0, "Task 0", 1, 4);
	CreateTask(1, "Task 1", 2, 6);
	CreateTask(2, "Task 2", 5, 12);

	/* Start the tasks running. */
	vTaskStartScheduler();

	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

static void DoNothingTask( void *pvParameters )
{
	portTickType xNextWakeTime;

	/* Initialise xNextWakeTime - this only needs to be done once. */
	xNextWakeTime = xTaskGetTickCount();

	for( ;; )
	{
		//Wake up every 150 ms and... do nothing!
		vTaskDelayUntil( &xNextWakeTime, MILLISECONDS(150) );
	}
}

/*-----------------------------------------------------------*/

int fputc( int iChar, FILE *pxNotUsed ) 
{
	/* Just to avoid compiler warnings. */
	( void ) pxNotUsed;

	if( mainDEMCR & mainTRCENA ) 
	{
		while( mainITM_Port32( 0 ) == 0 );
		mainITM_Port8( 0 ) = iChar;
  	}

  	return( iChar );
}
