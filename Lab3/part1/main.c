#include <lpc17xx.h>
#include "../mcb1700_ece/glcd.h"

#include "../common/led.h"
#include "../common/timer.h"
#include "../common/pushbutton.h"
#include "../common/debouncetest.h"

#include <stdio.h>

int _go = 1;

int main(void)
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);

	LED_Init();
	Pushbutton_Init();
	Timer_Init(LPC_TIM0);

	LED_On(1);

	Timer_Start(LPC_TIM0);
	while(_go)
	{
		unsigned char strbuf[20];
		snprintf((char*)strbuf, 20, "%d", Timer_GetTicks(LPC_TIM0));
		GLCD_DisplayString(1, 0, 1, strbuf);
	}

	return 0;
}

void EINT3_IRQHandler(void)
{
	Pushbutton_ClearInterrupt();
	_go = 0;

	LED_On(0);
}

/*
void GLCD_Init(void);
void GLCD_SetTextColor(unsigned short color);
void GLCD_SetBackColor(unsigned short color);
void GLCD_DisplayChar(unsigned int row, unsigned int column,
unsigned char font, unsigned char c);
void GLCD_DisplayString(unsigned int row, unsigned int column,
unsigned char font, unsigned char *s);
void GLCD_Clear(unsigned short color);
void GLCD_ClearLn(unsigned int row, unsigned char font);
void GLCD_PutPixel(x, y); // uses current foreground (text) colour
void GLCD_Bitmap(unsigned int x, unsigned int y,
unsigned int w, unsigned int h, unsigned char *bitmap);
void GLCD_ScrollVertical(unsigned int delta_y);

The colors can be the
predefined constants Black, Navy, DarkGreen, DarkCyan, Maroon, Purple, Olive, LightGrey,
DarkGrey, Blue, Green, Cyan, Red, Magenta, Yellow and White, or any RGB value packed into
16 bits in 5:6:5 format (i.e. red in bits 15:11, green in bits 10:5 and blue in bits 4:0).
*/
