#ifndef FSM_TYPES_H
#define FSM_TYPES_H

#include "fsm_config.h"

typedef unsigned int FSMInput;
typedef unsigned int StateID;

typedef struct
{
	StateID state_id;
	char *output;
	int has_output;
	StateID state_transition[FSM_NUM_INPUT_TYPES];
} FSMState;

#endif
