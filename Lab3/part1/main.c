#include <lpc17xx.h>
#include "../mcb1700_ece/glcd.h"

#include "../common/led.h"
#include "../common/timer.h"
//#include "../common/pushbutton.h"
//#include "../common/debouncetest.h"

int main(void)
{
	SystemInit();
	GLCD_Init();
	GLCD_Clear(White);

	LED_Init();
	//Pushbutton_Init();
	Timer_Init(LPC_TIM0);


	LPC_PINCON->PINSEL4 &= ~(3<<20); // P2.10 is GPIO
	LPC_GPIO2->FIODIR &= ~(1<<10); // P2.10 is input

	LPC_GPIOINT->IO2IntEnF |= 1 << 10; // falling edge of P2.10
	NVIC_EnableIRQ(EINT3_IRQn);

	LED_On(1);

	return 0;
}

void EINT3_IRQHandler(void)
{
	//Pushbutton_ClearInterrupt();
	LPC_GPIOINT->IO2IntClr |= 1 << 10; // clear interrupt condition

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
