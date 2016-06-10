#include "fsm_runner.h"
#include "fsm.h"

void FSMRunner_Init(FSMRunner *runner)
{
	runner->cur_state = FSM_GetState(0); //start at the initial state
	runner->has_output = 0;
}

int FSMRunner_ApplyInput(FSMRunner *runner, FSMInput new_input)
{
	if(!FSM_HasNextState(runner->cur_state, new_input))
	{
		return 1; //no state to transition to
	}

	runner->cur_state = FSM_GetNextState(runner->cur_state, new_input);
	if(runner->cur_state->has_output)
	{
		runner->has_output = 1;
		runner->cur_output = runner->cur_state->output;
	}
	else
	{
		runner->has_output = 0;
	}

	return 0;
}
