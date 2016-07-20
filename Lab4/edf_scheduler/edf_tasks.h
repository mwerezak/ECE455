#ifndef EDF_TASKS_H
#define EDF_TASKS_H

#include "FreeRTOS.h"
#include "task.h"

#define UNINTERRUPTIBLE_TIME 100 //duration in ms of each uninterruptible "unit" of work

#define NUM_TASKS 3

typedef portTickType Tick;
typedef unsigned portBASE_TYPE TaskPriority;

typedef unsigned int TaskID;

typedef struct
{
	TaskID id;
	int deadline_period; //ms
	int work_required; //work units
	int missed_count; //number of missed deadlines

	xTaskHandle handle;
	Tick last_deadline;
} TaskInfo;

void CreateTask(TaskID id, char *name, int exec_time, int period);
void InitTask( void *pvParameters );

static void RunTask( void *pvParameters );
static void UpdatePriorities(void);
static int CheckDeadline(TaskInfo *task);
static __inline Tick NextDeadline(TaskInfo *task);
static __inline void DoWork(void);

#endif
