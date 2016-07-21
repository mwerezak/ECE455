#ifndef EDF_TASKS_H
#define EDF_TASKS_H

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

//Timing conversions
#define TICKS(ms) (ms / portTICK_RATE_MS)
#define MILLISECONDS(ticks) (ticks * portTICK_RATE_MS)

#define WORK_PERIOD 10 //duration in ms of each unit of work

#define NUM_TASKS 3

typedef portTickType Tick;
typedef unsigned portBASE_TYPE TaskPriority;

typedef unsigned int TaskID;

typedef struct
{
	TaskID id;
	int deadline_period; //ms
	int deadline_period2; //ms
	int work_required; //work units
	int missed_count; //number of missed deadlines

	//for debugging
	TaskPriority priority;
	long last_slack;

	xTaskHandle handle;
	xTimerHandle timer;
	Tick last_deadline;
	int working;
	int work_done;
} TaskInfo;

void CreateTask(char *name, int exec_time, int period);
void InitTask( void *pvParameters );

static void RunTask( void *pvParameters );
static void UpdatePriorities(void);
static __inline Tick NextDeadline(TaskInfo *task);
static __inline void DoWork(TaskInfo *task);
static void WorkDone(xTimerHandle timer);

#endif
