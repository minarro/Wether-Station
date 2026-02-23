#include "lcd-st7066u.h"
#include <xc.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "delays.h"
#include "lcd-graphics.h"

/* Timing from ST7066U datasheet */
#define LCD_RESET_ms 40
#define LCD_SLOW_INST_ms 5 // Allowing for clock tolerance
#define LCD_FAST_INST_us 100 // Allowing for clock tolerance
#define LCD_E_HIGH_ns 480
#define LCD_E_LOW_ns (1200 - LCD_E_HIGH_ns)

/* Commands */
#define LCD_CLEAR 1
#define LCD_HOME 2
#define LCD_ENTRY_SET 4
#define LCD_DISP_CUR_SET 8
#define LCD_DISP_CUR_SHIFT 16
#define LCD_FN_SET 32
#define LCD_CGRAM_ADDR_SET 64
#define LCD_DISP_ADDR_SET 128

/* The following options to be bitwise-ORed with the relevant command */

/* LCD_ENTRY_SET options */
#define LCD_INCR 2
#define LCD_DECR 0
#define LCD_SHIFT_ON 1
#define LCD_SHIFT_OFF 0

/* LCD_DISP_CUR_SET options */
#define LCD_ON 4
#define LCD_OFF 0
#define LCD_CURSOR_ON 2
#define LCD_CURSOR_OFF 0
#define LCD_BLINK_ON 1
#define LCD_BLINK_OFF 0

/* LCD_DISP_CUR_SHIFT options */
#define LCD_DISP_SHIFT 8
#define LCD_CURSOR_SHIFT 0
#define LCD_RIGHT_SHIFT 4
#define LCD_LEFT_SHIFT 0

/* LCD_FN_SET options */
#define LCD_8BIT 16 // not supported by this library
#define LCD_4BIT 0
#define LCD_2LINE 8
#define LCD_1LINE 0
#define LCD_5X11 4
#define LCD_5X8 0

/* RS and RW values */
#define LCD_DAT 1
#define LCD_CMD 0
#define LCD_READ 1
#define LCD_WRITE 0

/* Private variables */
static uint8_t entry_mode = LCD_INCR | LCD_SHIFT_OFF;
static uint8_t display_mode = LCD_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;

/* Private functions */

static uint8_t lcd_readNibble(uint8_t regSel)
{
	LCD_RS_LAT = regSel;
	LCD_RW_LAT = LCD_READ;
	lcd_setDataTris();

	LCD_E_LAT = 1;
	delay_ns(LCD_E_HIGH_ns);
	uint8_t nibble = lcd_readDataPort();
	LCD_E_LAT = 0;
	delay_ns(LCD_E_LOW_ns);

	return nibble;
}

static uint8_t lcd_readByte(uint8_t regSel)
{
	return (uint8_t)(lcd_readNibble(regSel) | (lcd_readNibble(regSel) << 4));
}
static bool lcd_isBusy(void)
{
	return (bool)(lcd_readByte(LCD_CMD) & 0x80);
}

static void lcd_writeNibble(uint8_t regSel, uint8_t nibble)
{
	LCD_RS_LAT = regSel;
	LCD_RW_LAT = LCD_WRITE;
	lcd_clearDataTris();
	lcd_writeDataLat(nibble);

	LCD_E_LAT = 1;
	delay_ns(LCD_E_HIGH_ns);
	LCD_E_LAT  = 0;
	delay_ns(LCD_E_LOW_ns);
}

static void lcd_writeByte(uint8_t regSel, uint8_t byte)
{
	lcd_writeNibble(regSel, byte >> 4);
	lcd_writeNibble(regSel, byte & 0x0F);
}

static void lcd_writeCmd(uint8_t command)
{
	while(lcd_isBusy()) continue;
	lcd_writeByte(LCD_CMD, command);
}

static void lcd_writeData(uint8_t data)
{
	while(lcd_isBusy()) continue;
	lcd_writeByte(LCD_DAT, data);
}

static void lcd_initFnSet(uint8_t busMode)
{
	lcd_writeNibble(LCD_CMD, (LCD_FN_SET | busMode) >> 4);
	__delay_us(LCD_FAST_INST_us); // Wait for completion - no busy check
}

/* Public functions */

void lcd_init(bool twoLines, bool fiveByElevenDots)
{
	uint8_t lcdType = (uint8_t)((twoLines << 3) | (fiveByElevenDots << 2));
	LCD_RS_LAT = 0;
	LCD_RW_LAT = 0;
	LCD_E_LAT = 0;
	lcd_writeDataLat(0);

	LCD_RS_TRIS = 0;
	LCD_RW_TRIS = 0;
	LCD_E_TRIS = 0;
	lcd_clearDataTris();

	__delay_ms(LCD_RESET_ms);

	/* The datasheet assumes we started from POR which might not be true e.g. if the micro is reset.
	 * The following attemps to put us in a known state no matter when lcd_init was called.
	 */

	 lcd_initFnSet(LCD_8BIT); // Potentially consumed as 2nd half of a 4-bit mode instruction
	 lcd_initFnSet(LCD_8BIT); // This would then be the 1st half of the next instruction
	 lcd_initFnSet(LCD_8BIT); // Definitely in 8-bit mode now
	 lcd_initFnSet(LCD_4BIT); // Full 8-bit mode instruction. Puts us in 4-bit mode

	 lcd_writeByte(LCD_CMD, LCD_FN_SET | LCD_4BIT | lcdType); // No busy check

	 lcd_writeCmd(LCD_DISP_CUR_SET | display_mode);
	 lcd_writeCmd(LCD_CLEAR);
	 lcd_writeCmd(LCD_ENTRY_SET | entry_mode);
}

void lcd_loadGraphics(uint8_t const *data, uint8_t len)
{
	lcd_writeCmd(LCD_CGRAM_ADDR_SET);
	for(uint8_t i = 0; i < len; i++)
	{
		lcd_writeData((uint8_t)data[i]);	
	}
	lcd_writeCmd(LCD_DISP_ADDR_SET);
}

void lcd_loadDefaultGraphics(void)
{
	lcd_loadGraphics((uint8_t const *)lcd_default_graphics, LCD_DEFAULT_GRAPHICS_LEN);
}

void lcd_clear(void)
{
	lcd_writeCmd(LCD_CLEAR);
}

void lcd_home(void)
{
	lcd_writeCmd(LCD_HOME);
}

void lcd_useIncrementMode(void)
{
	entry_mode |= LCD_INCR;
	lcd_writeCmd(LCD_ENTRY_SET | entry_mode);
}

void lcd_useDecrementMode(void)
{
	entry_mode &= ~LCD_INCR;
	lcd_writeCmd(LCD_ENTRY_SET | entry_mode);
}

void lcd_shiftOn(void)
{
	entry_mode |= LCD_SHIFT_ON;
	lcd_writeCmd(LCD_ENTRY_SET | entry_mode);
}

void lcd_shiftOff(void)
{
	entry_mode &= ~LCD_SHIFT_ON;
	lcd_writeCmd(LCD_ENTRY_SET | entry_mode);
}

void lcd_on(void)
{
	display_mode |= LCD_ON;
	lcd_writeCmd(LCD_DISP_CUR_SET | display_mode);
}

void lcd_off(void)
{
	display_mode &= ~LCD_ON;
	lcd_writeCmd(LCD_DISP_CUR_SET | display_mode);
}

void lcd_cursorOn(void)
{
	display_mode |= LCD_CURSOR_ON;
	lcd_writeCmd(LCD_DISP_CUR_SET | display_mode);
}

void lcd_cursorOff(void)
{
	display_mode &= ~LCD_CURSOR_ON;
	lcd_writeCmd(LCD_DISP_CUR_SET | display_mode);
}

void lcd_blinkOn(void)
{
	display_mode |= LCD_BLINK_ON;
	lcd_writeCmd(LCD_DISP_CUR_SET | display_mode);
}

void lcd_blinkOff(void)
{
	display_mode &= ~LCD_BLINK_ON;
	lcd_writeCmd(LCD_DISP_CUR_SET | display_mode);
}

void lcd_shiftDisplay(int8_t amount)
{
	uint8_t direction = LCD_LEFT_SHIFT;
	if(amount < 0)
	{
		direction = LCD_RIGHT_SHIFT;
		amount = -amount;
	}

	for(int8_t i = 0; i < amount; i++)
	{
		lcd_writeCmd(LCD_DISP_CUR_SHIFT | LCD_DISP_SHIFT | direction);
	}
}

void lcd_shiftCursor(int8_t amount)
{
	uint8_t direction = LCD_RIGHT_SHIFT;
	if(amount < 0)
	{
		direction = LCD_LEFT_SHIFT;
		amount = -amount;
	}

	for(int8_t i = 0; i < amount; i++)
	{
		lcd_writeCmd(LCD_DISP_CUR_SHIFT | LCD_CURSOR_SHIFT | direction);
	}
}

uint8_t lcd_readAddress(void)
{
	return lcd_readByte(LCD_CMD) & 0x7F;
}

void lcd_move(uint8_t address)
{
	lcd_writeCmd(LCD_DISP_ADDR_SET | address);
}

void lcd_putc(char c)
{
	lcd_writeData((uint8_t)c);
}

void lcd_puts(char const *s)
{
	char c;
	while((c = *s++)) lcd_putc(c);
}

void lcd_printf(char const *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	#define LCD_BUFSIZE (LCD_LINE_LEN + 1)
	char buf[LCD_BUFSIZE];
	vsnprintf(buf, LCD_BUFSIZE, fmt, ap);
	lcd_puts(buf);
	va_end(ap);
}
