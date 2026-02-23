#include "spi.h"
#include <xc.h>
#include <stdint.h>

void spi_setup(void)
{
	SSP1STATbits.SMP = 1; // Sample at end of output time
	SSP1STATbits.CKE = 1; // Transmit on active-to-idle
	SSP1CON1 = 0x30;  // On, idle high, master mode Fosc/4 
	ODCON3bits.SPI1OD = 0; // Open drain disabled (capacitance of breadboard too high?)
	SPI_MISO_TRIS = 1;
	SPI_MOSI_TRIS = 0;
	SPI_SCK_TRIS = 0;
}

uint8_t spi_send(uint8_t byte)
{
	SSP1BUF = byte;
	while(!SSP1STATbits.BF) continue;
	return SSP1BUF;
}