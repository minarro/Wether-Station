#ifndef LCD_ST7066U_H_
#define LCD_ST7066U_H_

#include <stdbool.h>
#include <stdint.h>
#include "lcd-config.h"

void lcd_init(bool twoLines, bool fiveByElevenDots); // 8-bit mode not supported
void lcd_loadGraphics(uint8_t const *data, uint8_t len); // See lcd-graphics.c for example data
void lcd_loadDefaultGraphics(void);

void lcd_clear(void);
void lcd_home(void);
void lcd_useIncrementMode(void);
void lcd_useDecrementMode(void);
void lcd_shiftOn(void);
void lcd_shiftOff(void);
void lcd_on(void);
void lcd_off(void);
void lcd_cursorOn(void);
void lcd_cursorOff(void);
void lcd_blinkOn(void);
void lcd_blinkOff(void);
void lcd_shiftDisplay(int8_t amount); // Negative amount to shift left
void lcd_shiftCursor(int8_t amount); // Negative amount to shift left

uint8_t lcd_readAddress(void);
void lcd_move(uint8_t address);

void lcd_putc(char c);
void lcd_puts(char const *c);
// lcd_printf is provided for completeness 
// but printf is recommended as it is more versatile and uses less program memory
void lcd_printf(char const *fmt, ...);

#endif
