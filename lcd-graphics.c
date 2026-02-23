#include "lcd-graphics.h"
#include <stdint.h>

uint8_t const lcd_default_graphics[LCD_NGRAPH][LCD_GRAPH_HEIGHT] =
{
	['\0'] = {0, 0, 0, 0, 0, 0, 0, 0},
	[POUND_CHR] = {6, 9, 8, 30, 8, 8, 31, 0},
	[EURO_CHR] = {6, 9, 28, 8, 28, 9, 6, 0},
	[PLUSMINUS_CHR] = {4, 4, 31, 4, 4, 0, 31, 0},
	[SQUARED_CHR] = {28, 4, 28, 16, 28, 0, 0, 0},
	[CUBED_CHR] = {28, 4, 28, 4, 28, 0, 0, 0},
	[UARROW_CHR] = {4, 14, 21, 4, 4, 4, 4, 0},
	[DARROW_CHR] = {4, 4, 4, 4, 21, 14, 4, 0},
};