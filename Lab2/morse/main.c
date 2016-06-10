#include <lpc17xx.h>
#include "../mcb1700_ece/glcd.h"

#include "../common/led.h"
#include "../common/pushbutton.h"
#include "../common/morsereader.h"
#include "../fsm/fsm.h"

int CreateMorseFSM(void);

int main()
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);

	LED_Init();
	Pushbutton_Init();
	Timer_Init();

	FSM_Init();
	CreateMorseFSM();

	MorseReader_Init();
	MorseReader_TestRun();

	return 0;
}


//Label the possible inputs for readability
#define DOT  0
#define DASH 1
static int CreateMorseFSM(void)
{
	FSMInput A[2] = {DOT, DASH};
	FSMInput B[4] = {DASH, DOT, DOT, DOT};
	FSMInput C[4] = {DASH, DOT, DASH, DOT};
	FSMInput D[3] = {DASH, DOT, DOT};
	FSMInput E[1] = {DOT};

	FSM_CreateInputSequence("A", 2, A);
	FSM_CreateInputSequence("B", 4, B);
	FSM_CreateInputSequence("C", 4, C);
	FSM_CreateInputSequence("D", 3, D);
	FSM_CreateInputSequence("E", 1, E);

	return 0;
}
#undef DOT
#undef DASH
