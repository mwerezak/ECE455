#ifndef EDF_TASKS_H
#define EDF_TASKS_H

#include "FreeRTOS.h"
#include "task.h"

#define UNINTERRUPTIBLE_TIME 10 //duration in ms of each uninterruptible "unit" of work

#define NUM_TASKS 3

typedef portTickType Tick;
typedef unsigned portBASE_TYPE TaskPriority;

typedef unsigned int TaskID;

typedef struct TaskInfo
{
	TaskID id;
	int deadline_period; //ms
	int work_required; //work units
	int missed_count; //number of missed deadlines

	xTaskHandle handle;
	Tick last_deadline;
} TaskInfo;

void CreateTask(TaskID id, char *name, int exec_time, int period);

static __inline void DoWork(void);
static void RunTask( void *pvParameters );

#endif
