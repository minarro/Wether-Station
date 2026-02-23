#include "bme280-interface.h"
#include <xc.h>
#include <stdint.h>
#include "spi.h"

#define CSB LATCbits.LC6 // Note: ensure corresponding TRIS bit is zero

uint8_t bme_read(uint8_t addr)
{
	CSB = 0;
	spi_send(addr);
	uint8_t data = spi_send(0);
	CSB = 1;
	return data;
}

void bme_write(uint8_t addr, uint8_t data)
{
	CSB = 0;
	spi_send(addr & 0x7F);
	spi_send(data);
	CSB = 1;
}

uint8_t *bme_burstRead(uint8_t reg, uint8_t len, uint8_t data[])
{
	CSB = 0;
	spi_send(reg);
	for(uint8_t i = 0; i < len; i++)
	{
		data[i] = spi_send(++reg);
	}
	CSB = 1;
	return data;
}
