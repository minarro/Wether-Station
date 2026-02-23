/*Modify this file to suit your hardware */

#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

#include <xc.h>
#include <stdint.h>

/*Line length, count and start addresses */
#define LCD_LINE_LEN 20
#define LCD_LINE_CNT 4
#define LCD_LINE1 0
#define LCD_LINE2 64
#define LCD_LINE3 (LCD_LINE1 + LCD_LINE_LEN)
#define LCD_LINE4 (LCD_LINE2 + LCD_LINE_LEN)

/* Control lines */
#define LCD_RS_LAT LATAbits.LATA5
#define LCD_RW_LAT LATAbits.LATA6
#define LCD_E_LAT LATAbits.LATA7

#define LCD_RS_TRIS TRISAbits.TRISA5
#define LCD_RW_TRIS TRISAbits.TRISA6
#define LCD_E_TRIS TRISAbits.TRISA7

/* Data lines */
static inline void lcd_clearDataTris(void)
{
	TRISA &= 0xF0;
}

static inline void lcd_setDataTris(void)
{
	TRISA |= 0x0F;
}

static inline uint8_t lcd_readDataPort(void)
{
	return PORTA & 0x0F;
}

static inline void lcd_writeDataLat(uint8_t nibble)
{
	LATA = (LATA & 0xF0) | (nibble & 0x0F);
}

#endif
