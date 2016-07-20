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
	task->work_required = exec_time*1000/UNIT_OF_WORK_MS;
	task->missed_count = 0;
	task->last_deadline = 0;
	task->last_slack = 0;

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
	while(!sorted); //bubble sort

	//now set priorities, first task in the sorted array should have least priority
	taskENTER_CRITICAL();
	for(i = 0; i < _active_tasks; i++)
	{
		sorted_tasks[i]->priority = i+1;
		vTaskPrioritySet(sorted_tasks[i]->handle, i+1);
	}
	taskEXIT_CRITICAL();
}

static void RunTask( void *pvParameters )
{
	TaskInfo *this = (TaskInfo *) pvParameters;
	Tick last_wake_time = xTaskGetTickCount();
	Tick next_deadline;

	while(1)
	{
		//start doing work
		this->work_done = 0;
		next_deadline = NextDeadline(this);
		while(this->work_done < this->work_required)
		{
			//since the system is EDF, we only need to check the current task
			if(xTaskGetTickCount() >= next_deadline)
			{
				this->missed_count++;
				break;
			}

			//Do 1 unit of work
			DoWork();
			this->work_done++;
		}

		this->last_slack = next_deadline - xTaskGetTickCount();
		this->last_deadline = next_deadline; //set the deadline for the next period of work
		UpdatePriorities(); //re-evaluate priorities

		//sleep until the next period
		if(this->last_deadline > last_wake_time)
		{
			vTaskDelayUntil( &last_wake_time, this->last_deadline - last_wake_time );
		}
	}
}

static __inline Tick NextDeadline(TaskInfo *task)
{
	return task->last_deadline + TICKS(task->deadline_period);
}

static __inline void DoWork(void)
{
	int i;
	for(i = 0; i < UNIT_OF_WORK_MS/7*configCPU_CLOCK_HZ/1000; i++);
}
