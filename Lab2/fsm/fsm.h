#ifndef FSM_H
#define FSM_H

#include "fsm_types.h"

void FSM_Init(void);
int FSM_CreateInputSequence(char *seq_name, int seq_length, FSMInput input_sequence[]);
int FSM_HasNextState(FSMState *cur_state, FSMInput input);
FSMState *FSM_GetNextState(FSMState *cur_state, FSMInput input);
FSMState *FSM_GetState(int state_id);

static FSMState *FSM_CreateState(char *output);
static int FSM_AddStateTransition(FSMState *from_state, FSMState *to_state, FSMInput trigger);
static int FSM_AddOutput(FSMState *state, char *new_output);

#endif
