#include "fsm.h"
#include "fsm_error.h"

//A dynamically configured FSM, represented as an n-ary tree stored in an array. 

static FSMState _fsm_state_store[FSM_MAX_STATES];
static unsigned int _num_states;

#define UNUSED 0

void FSM_Init(void)
{
	_num_states = 0;

	FSM_CreateState(0);
}

static FSMState *FSM_CreateState(char *output)
{
	int i;
	StateID new_state_idx = _num_states++;
	FSMState *new_state;

	if(new_state_idx >= FSM_MAX_STATES)
	{
		FSM_RaiseError("too many states");
		return 0;
	}

	new_state = &_fsm_state_store[new_state_idx];
	new_state->state_id = new_state_idx;
	if(output != 0)
	{
		new_state->output = output;
		new_state->has_output = 1;
	}
	else
	{
		new_state->has_output = 0;
	}
	
	for(i = 0; i < FSM_NUM_INPUT_TYPES; i++)
	{
		new_state->state_transition[i] = UNUSED;
	}

	return new_state;
}

FSMState *FSM_GetState(int state_id)
{
	return &_fsm_state_store[state_id];
}

int FSM_HasNextState(FSMState *cur_state, FSMInput input)
{
	StateID next_state_id = cur_state->state_transition[input];
	return (next_state_id != UNUSED);
}

FSMState *FSM_GetNextState(FSMState *cur_state, FSMInput input)
{
	StateID next_state_id = cur_state->state_transition[input];
	if(next_state_id != UNUSED)
	{
		return FSM_GetState(next_state_id);
	}
	return 0;
}

static int FSM_AddStateTransition(FSMState *from_state, FSMState *to_state, FSMInput trigger)
{
	if(from_state->state_transition[trigger] != UNUSED)
	{
		FSM_RaiseError("overwrite transition");
		return 2;
	}

	from_state->state_transition[trigger] = to_state->state_id;
	return 0;
}

static int FSM_AddOutput(FSMState *state, char *new_output)
{
	if(state->has_output)
	{
		FSM_RaiseError("conflicting output");
		return 1;
	}
	state->output = new_output;
	state->has_output = 1;
	return 0;
}

int FSM_CreateInputSequence(char *seq_output, int seq_length, FSMInput input_sequence[])
{
	int input_idx;
	FSMInput this_input;
	FSMState *next_state, *cur_state = FSM_GetState(0); //start at the initial state

	//enter the input sequence into the FSM, up to the last input
	for(input_idx = 0; input_idx < seq_length; input_idx++)
	{
		this_input = input_sequence[input_idx];

		if(input_idx < seq_length - 1) //intermediate state
		{
			if(FSM_HasNextState(cur_state, this_input))
			{
				next_state = FSM_GetNextState(cur_state, this_input);
			}
			else
			{
				next_state = FSM_CreateState(0);
				FSM_AddStateTransition(cur_state, next_state, this_input);
			}
		}
		else //add the output to the last state
		{
			if(FSM_HasNextState(cur_state, this_input))
			{
				next_state = FSM_GetNextState(cur_state, this_input);
				FSM_AddOutput(next_state, seq_output);
			}
			else
			{
				next_state = FSM_CreateState(seq_output);
				FSM_AddStateTransition(cur_state, next_state, this_input);
			}
		}

		cur_state = next_state;
	}

	return 0; //should probably check for errors, but that would make this messy
}

