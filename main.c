/* Weather station (temperature, humidity, pressure) using PIC18F27J53, BME280 and a 4x20 character LCD
 * 26/8/25
 */

#pragma config XINST=OFF /*not supported by XC8*/, STVREN=OFF, CFGPLLEN=OFF, PLLDIV=1, WDTEN=OFF /*controlled by SWDTEN!*/
#pragma config CP0=OFF, CPUDIV=OSC1
#pragma config IESO=OFF, FCMEN=OFF, CLKOEC=OFF, SOSCSEL=HIGH /*alt: LOW or DIG*/, OSC=INTOSC
#pragma config WDTPS=32768
#pragma config DSWDTPS=G2, DSWDTEN=OFF, DSBOREN=OFF, RTCOSC=T1OSCREF, DSWDTOSC=INTOSCREF
#pragma config MSSP7B_EN=MSK7, ADCSEL=BIT12, IOL1WAY=ON
#pragma config WPCFG=ON, WPFP=PAGE_0
#pragma config LS48MHZ=SYS48X8 /*bypassed in full-speed mode*/, WPEND=PAGE_WPFP, WPDIS=ON

#include <xc.h>
#include <stdio.h>
#include "adc.h"
#include "bme280.h"
#include "delays.h"
#include "hardware.h"
#include "lcd-graphics.h"
#include "lcd-st7066u.h"
#include "xtal.h"

void main(void)
{
	setup();
	lcd_init(true, false);
	bme_init(); // Read out calibration coefficients (call once)

#ifdef USE_NORMAL_MODE // Use the following procedure for automatic sampling
	bme_setStandbyTime(BME_1s);
	bme_setCtrl(BME_SAMPLEx16, BME_NORMAL);
	while(!bme_isMeasuring()) continue; // Status bit not set immediately
	while(bme_isMeasuring()) continue; // Wait for first data
#endif

	while(1)
	{
		float t, h, p;

#ifdef USE_NORMAL_MODE // Automatic sampling. Read data asynchronously
		bme_getData(&t, &h, &p);
#else // Manual sampling. Blocks till data is ready (~10 ms)
		bme_getSample(&t, &h, &p);
#endif

		unsigned v = adc_getVdd_mV();

		printf(
			"Temp.    %5.1f    " DEGREE_STR "C\n"
			"Humid.   %5.1f   %%RH\n"
			"Press.   %7.3f kPa\n"
			"Supply   %6.2f    V\n",
			t,
			h,
			p,
			v / 1000.0f
		);
		longDelay_ms(10000); // Sleep here or do other tasks as necessary
	}
}
