#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#define PUSHBUTTON_VAL (~(LPC_GPIO2->FIOPIN >> 10) & 0x01)

void Pushbutton_Init(void);
void Pushbutton_WaitForEvent(void);

#endif
