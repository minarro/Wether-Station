#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

#define BANDGAP_mV 1200
#define BATT_OFFSET_mV 90

uint16_t adc_getVdd_mV(void);

#endif
