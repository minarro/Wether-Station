#ifndef BME280_INTERFACE_H_
#define BME280_INTERFACE_H_

#include <stdint.h>

uint8_t bme_read(uint8_t addr);
void bme_write(uint8_t addr, uint8_t data);
uint8_t *bme_burstRead(uint8_t reg, uint8_t len, uint8_t data[]);

#endif