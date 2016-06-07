

#include "../mcb1700_ece/glcd.h"
#include <stdio.h>
#include <stdarg.h>

#define LINE_SIZE 20

void LCD_Init(void)
{
	GLCD_Init();
	GLCD_Clear(White);
}

void LCD_Print(unsigned int row, unsigned int col, const char* format, ...)
{
	char linebuf[LINE_SIZE];

	va_list args;
	va_start(args, format);
	vsprintf(linebuf, format, args)
	va_end(args);
}

