#ifndef DEBOUNCETEST_H
#define DEBOUNCETEST_H

//*** DebounceTest
//#define DEBOUNCETEST_ENABLE

void DebounceTest_Init(void);
void DebounceTest_Reset(void);
void DebounceTest_Run(void);
static void DebounceTest_PrintInfo(void);
static void DebounceTest_HandleEdge(int edgeval);

#endif
