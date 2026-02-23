#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <xc.h>
#include "xtal.h"
#include "spi.h"

static inline void setup(void)
{
	OSCCONbits.IRCF = IRCF_SEL;
	OSCCONbits.SCS = 0b11;
	OSCCON2bits.PRISD = 0;

	WDTCONbits.REGSLP = 1;
	PMDIS3 = 0xFF;
	PMDIS2 = 0xFF;
	PMDIS1 = 0xFF;
	PMDIS0 = 0xFC; // SPI1 and ADC on
	UCONbits.USBEN = 0;
	UCFGbits.UTRDIS = 1;
	ANCON0 = 0xFF;
	ANCON1 = 0x1F;
	LATA = 0;
	LATB = 0;
	LATC = 0b01000000;
	TRISA = 0;
	TRISB = 0b00110000;
	TRISC = 0b10000000;

	spi_setup();
}

#endif