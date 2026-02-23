#ifndef DELAYS_H_
#define DELAYS_H_

#include <xc.h>
#include <stdint.h>
#include "xtal.h"

#define T_INST_ns (4000000000UL / _XTAL_FREQ)

/* Does NOT provide nanosecond resolution! */
#define delay_ns(X)\
do{\
	for(uint16_t i = 0; i < (uint16_t)(((X) + T_INST_ns - 1) / T_INST_ns); i++)\
	{\
		NOP();\
	}\
} while(0)

/*For delays exceeding the limit of 50463240 cycles (was 197120 on PIC18 in XC8 V1) */
#define longDelay_ms(X)\
do{\
	for(uint32_t i = 0; i < (X) / 10; i++)\
	{\
		__delay_ms(10);\
	}\
} while(0)

#endif