#ifndef FSM_RUNNER_H
#define FSM_RUNNER_H

#include "fsm_types.h"

typedef struct
{
	FSMState *cur_state;
	int has_output;
	char *cur_output;
} FSMRunner;

void FSMRunner_Init(FSMRunner *runner);
int FSMRunner_ApplyInput(FSMRunner *runner, FSMInput new_input);

#endif
