#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "timer.h"

#define INPUT_PRESSED 0
#define INPUT_RAISED  1

#define DEBOUNCE_READY     0
#define DEBOUNCE_PRESSED   1

#define DEBOUNCE_DELAY 60 //ms during which additional events will be ignored

typedef void (*DebouncedInputHandler)(int input_event);

//*** Debounce
#define DEBOUNCE_ENABLE

void Debounce_Init(LPC_TIMER *debounce_timer, DebouncedInputHandler input_handler);
void Debounce_Reset(void);
static void Debounce_HandlePushbutton(void);

#endif
