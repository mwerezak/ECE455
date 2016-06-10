#ifndef MORSEREADER_H
#define MORSEREADER_H

#define INPUT_NEW  0
#define INPUT_DOT  1
#define INPUT_DASH 2

#define DEBOUNCE_READY     0
#define DEBOUNCE_PRESSED   1

#define DEBOUNCE_DELAY 60 //ms during which additional events will be ignored
#define DASH_DELAY 500    //ms after which input will be registered as a dash

//*** MorseReader
#define MORSEREADER_ENABLE

void MorseReader_Init(void);
void MorseReader_Reset(void);
void MorseReader_TestRun(void);
void MorseReader_FSMRun(void);
static void MorseReader_UpdateLED(void);
static void MorseReader_RegisterInput(int input_type);
static void MorseReader_HandlePushbutton(void);

#endif
