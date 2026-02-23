#ifndef SPI_H_
#define SPI_H_

#include <xc.h>
#include <stdint.h>

#define SPI_MISO_TRIS TRISBbits.TRISB5
#define SPI_MOSI_TRIS TRISCbits.TRISC7
#define SPI_SCK_TRIS TRISBbits.TRISB4

void spi_setup(void);
uint8_t spi_send(uint8_t);

#endif