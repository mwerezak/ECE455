#include "edf_tasks.h"

#include "FreeRTOSConfig.h"

static TaskInfo _tasks_store[NUM_TASKS]; //where all the task info is stored

//exec_time, period is in seconds
void CreateTask(TaskID id, char *name, int exec_time, int period)
{
	TaskInfo *task = &_tasks_store[id];

	task->id = id;
	task->deadline_period = period*1000;
	task->work_required = exec_time*1000/UNINTERRUPTIBLE_TIME;
	task->missed_count = 0;

	xTaskCreate(RunTask, (const signed char *) name, configMINIMAL_STACK_SIZE, (void*) task, 1, &(task->handle));
}

static void RunTask( void *pvParameters )
{
	TaskInfo *this = (TaskInfo *) pvParameters;
	this->last_deadline = xTaskGetTickCount();

	while(1)
	{
		vTaskDelayUntil( &this->last_deadline, MILLISECONDS(this->deadline_period) );
	}
}

static __inline Tick TimeLeft(TaskInfo *task)
{
	Tick curtime = xTaskGetTickCount();
	return task->last_deadline + TICKS(task->deadline_period) - curtime;
}


//Do 1 unit of work, this takes a number of milliseconds equal to UNINTERRUPTIBLE_TIME
static __inline void DoWork(void)
{
	int i;

	taskENTER_CRITICAL();

	for(i = 0; i < UNINTERRUPTIBLE_TIME*configCPU_CLOCK_HZ/1000; i++);

	taskEXIT_CRITICAL();
}

/*
	portTickType xNextWakeTime;

	//Initialise xNextWakeTime - this only needs to be done once.
	xNextWakeTime = xTaskGetTickCount();

	for( ;; )
	{
		//Wake up every 150 ms and... do nothing!
		vTaskDelayUntil( &xNextWakeTime, MILLISECONDS(150) );
	}
*/


