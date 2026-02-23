#include <stdbool.h>
#include <stdint.h>
#include "lcd-st7066u.h"

/* This function is required by stdio functions like puts and printf */
void putch(char c)
{
	static uint8_t remLen = LCD_LINE_LEN;
	static uint8_t line = 1;
	static bool skipNewline = false;

	if(c == '\n')
	{
		if(skipNewline)
		{
			skipNewline = false;
			return;
		}

		for(; remLen; remLen--) lcd_putc(' ');
	}
	else
	{
		// Could use long-tailed letters here if LCD font size is 5x11
		//if(c == 'g' || c == 'j' || c == 'p' || c == 'q' || c == 'y') c += 128;
		lcd_putc(c);
		remLen--;
		skipNewline = false;
	}

	if(!remLen)
	{
		switch(line)
		{
			case 1:
			lcd_move(LCD_LINE2);
			break;
			case 2:
			lcd_move(LCD_LINE3);
			break;
			case 3:
			lcd_move(LCD_LINE4);
			break;
			case 4:
			lcd_move(LCD_LINE1);
			break;
		}

		if(++line == 5) line = 1;
		remLen = LCD_LINE_LEN;
		if(c != '\n') skipNewline = true;
	}
}