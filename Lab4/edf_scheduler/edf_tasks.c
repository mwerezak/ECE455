#include "edf_tasks.h"

#include "FreeRTOSConfig.h"

static int _active_tasks = 0;
static TaskInfo _tasks_store[NUM_TASKS]; //where all the task info is stored

//exec_time, period is in seconds
void CreateTask(TaskID id, char *name, int exec_time, int period)
{
	TaskInfo *task = &_tasks_store[id];

	task->id = id;
	task->deadline_period = period*1000;
	task->work_required = exec_time*1000/UNINTERRUPTIBLE_TIME;
	task->missed_count = 0;
	task->last_deadline = 0;

	//create all tasks with priority 1 initially, the priority will be set later by the InitTask
	xTaskCreate(RunTask, (const signed char *) name, configMINIMAL_STACK_SIZE, (void*) task, 1, &(task->handle));
	_active_tasks++;
}

//special task used to do initialization
void InitTask( void *pvParameters )
{
	int i;
	Tick init_time = xTaskGetTickCount();

	//init last deadline
	for(i = 0; i < _active_tasks; i++)
	{
		_tasks_store[i].last_deadline = init_time;
	}

	UpdatePriorities();
	vTaskDelete(NULL); //delete ourselves
}

static void UpdatePriorities(void)
{
	int i, sorted;
	TaskInfo *temp;
	TaskInfo* sorted_tasks[NUM_TASKS];
	Tick curtime = xTaskGetTickCount();

	//init an array of pointers to the active tasks
	for(i = 0; i < _active_tasks; i++)
	{
		sorted_tasks[i] = &_tasks_store[i];
	}

	//then sort it according to the amount of time left
	do
	{
		sorted = 1;
		for(i = 1; i < _active_tasks; i++)
		{
			//sort so that the task with the earliest deadline is last
			if(NextDeadline(sorted_tasks[i-1]) < NextDeadline(sorted_tasks[i]))
			{
				sorted = 0;
				temp = sorted_tasks[i];
				sorted_tasks[i] = sorted_tasks[i-1];
				sorted_tasks[i-1] = temp;
			}
		}
	}
	while(!sorted); //bubble sort lol

	//now set priorities, first task in the sorted array should have least priority
	for(i = 0; i < _active_tasks; i++)
	{
		vTaskPrioritySet(sorted_tasks[i]->handle, i+1);
	}
}

static void RunTask( void *pvParameters )
{
	int i, work_done;
	int missed;
	TaskInfo *this = (TaskInfo *) pvParameters;
	Tick last_wake_time = xTaskGetTickCount();

	while(1)
	{
		//start doing work
		work_done = 0;
		while(work_done < this->work_required)
		{
			DoWork();
			work_done++;

			//check deadlines for every task
			missed = 0;
			for(i = 0; i < _active_tasks; i++)
			{
				missed += CheckDeadline(&_tasks_store[i]);
			}
			if(missed > 0)
			{
				UpdatePriorities(); //if any of them missed, re-evaluate priorities
			}
		}

		this->last_deadline = NextDeadline(this); //set the deadline for the next period of work
		UpdatePriorities(); //re-evaluate priorities

		//sleep until the next period
		last_wake_time = this->last_deadline;
		vTaskDelayUntil( &last_wake_time, 0 );
	}
}

//Ensures that a task's deadline isn't in the past
static int CheckDeadline(TaskInfo *task)
{
	int missed = 0;
	Tick curtime = xTaskGetTickCount();

	while(curtime > NextDeadline(task))
	{
		missed++;
		task->last_deadline = NextDeadline(task);
	}

	task->missed_count += missed;
	return missed;
}

static __inline Tick NextDeadline(TaskInfo *task)
{
	return task->last_deadline + TICKS(task->deadline_period);
}


//Do 1 unit of work, this takes a number of milliseconds equal to UNINTERRUPTIBLE_TIME
static __inline void DoWork(void)
{
	int i;

	taskENTER_CRITICAL();

	for(i = 0; i < UNINTERRUPTIBLE_TIME/7*configCPU_CLOCK_HZ/1000; i++);

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


