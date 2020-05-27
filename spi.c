#include "rpi.h"

#define SPI_CS		((volatile uint*)(SPI_BASE + 0x00))
#define SPI_FIFO	((volatile uint*)(SPI_BASE + 0x04))
#define SPI_CLK		((volatile uint*)(SPI_BASE + 0x08))
#define SPI_DLEN	((volatile uint*)(SPI_BASE + 0x0C))
#define SPI_LTOH	((volatile uint*)(SPI_BASE + 0x10))
#define SPI_DC		((volatile uint*)(SPI_BASE + 0x14))

#define CS_TXD		(1 << 18)
#define CS_RXD		(1 << 17)
#define CS_DONE		(1 << 16)
#define CS_TA		(1 << 7)
#define CS_CLRFIFO	(3 << 4)

#define SCLK	11
#define MOSI	10
#define MISO	9
#define CS0		8
#define CS1		7


void spi_init() {
	gpio_mode(SCLK, GPIO_FN0);
	gpio_mode(MOSI, GPIO_FN0);
	gpio_mode(MISO, GPIO_FN0);
	gpio_mode(CS0, GPIO_FN0);
	gpio_mode(CS1, GPIO_FN0);
	*SPI_CS = 0;
}

void spi_readwrite(char *buff, uint len) {
	uint i;
	*SPI_CS = CS_CLRFIFO | CS_TA;

	for (i = 0; i < len; ++i) {
		while (!(*SPI_CS & CS_TXD));

		*SPI_FIFO = buff[i];

		while (!(*SPI_CS & CS_DONE));

		while (*SPI_CS & CS_RXD)
			buff[i] = *SPI_FIFO;
	}
	*SPI_CS = 0;
}
