#include "fsm.h"
#include "fsm_error.h"

//A dynamically configured FSM, represented as an n-ary tree stored in an array. 

static FSMState _fsm_state_store[FSM_MAX_STATES];
static unsigned int _num_states;

#define UNUSED 0

void FSM_Init(void)
{
	_num_states = 0;

	FSM_CreateState("Initial State", 0);
}

static FSMState *FSM_CreateState(char *name, int is_final)
{
	int i;
	StateID new_state_idx = _num_states++;

	FSMState *new_state = &_fsm_state_store[new_state_idx];
	new_state->name = name;
	new_state->is_final = is_final;
	new_state->state_id = new_state_idx;
	for(i = 0; i < FSM_NUM_INPUT_TYPES; i++)
	{
		new_state->next_ids[i] = UNUSED;
	}

	return new_state;
}

static FSMState *FSM_GetState(int state_id)
{
	return &_fsm_state_store[state_id];
}

void FSM_CreateInputSequence(char *seq_name, int seq_length, InputID input_sequence[])
{
	int cur_input;
	InputID this_input;
	StateID next_state_id;
	FSMState *new_state, *cur_state = FSM_GetState(0); //start at the initial state

	//enter the input sequence into the FSM, up to the last input
	for(cur_input = 0; cur_input < seq_length - 1; cur_input++)
	{
		this_input = input_sequence[cur_input];
		next_state_id = cur_state->next_ids[this_input];

		if(next_state_id == 0)
		{
			new_state = FSM_CreateState("Intermediate State", 0);
			cur_state->next_ids[this_input] = new_state->state_id;
			cur_state = new_state;
		}
		else
		{
			cur_state = FSM_GetState(next_state_id);
		}
	}

	//The final input to the FSM should then result in an end state
	this_input = input_sequence[cur_input];
	next_state_id = cur_state->next_ids[this_input];

	new_state = FSM_CreateState(seq_name, 1);
	cur_state->next_ids[this_input] = new_state->state_id;
}

