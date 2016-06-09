#ifndef FSM_H
#define FSM_H

#include "fsm_config.h"

typedef unsigned int InputID;
typedef unsigned int StateID;

typedef struct
{
	StateID state_id;
	char *name;
	int is_final;
	StateID next_ids[FSM_NUM_INPUT_TYPES];
} FSMState;

void FSM_Init(void);
static FSMState *FSM_CreateState(char *name, int is_final);


#endif
