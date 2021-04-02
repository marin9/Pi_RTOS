#include "rpi.h"

#define SPI_CS		((volatile uint*)(SPI_BASE + 0x00))
#define SPI_FIFO	((volatile uint*)(SPI_BASE + 0x04))
#define SPI_CLK		((volatile uint*)(SPI_BASE + 0x08))
#define SPI_DLEN	((volatile uint*)(SPI_BASE + 0x0C))

#define CS_CSPOL2	(1 << 23) // CS2 polarity
#define CS_CSPOL1	(1 << 22) // CS1 polarity
#define CS_CSPOL0	(1 << 21) // CS0 polarity
#define CS_RXF		(1 << 20) // RX FIFO full
#define CS_RXR		(1 << 19) // RX FIFO need reading
#define CS_TXD		(1 << 18) // TX has space
#define CS_RXD		(1 << 17) // RX not empty
#define CS_DONE		(1 << 16) // Transfer done
#define CS_INTR		(1 << 10) // Interrupt on RXR=1
#define CS_INTD		(1 << 9) // Interrupt on done=1
#define CS_TA		(1 << 7) // Transfer active
#define CS_CSPOL	(1 << 6) // CS active at
#define CS_CLRFIFO	(3 << 4) // Clear FIFO RX or TX
#define CS_CPOL		(1 << 3) // reset state of clock
#define CS_CPHA 	(1 << 2) // clock phase, first sclk transition
							 // 0 - middle of data bit
							 // 1 - beginning of data bit
#define CS_CS0		(0 << 0)
#define CS_CS1		(1 << 0)
#define CS_CS2		(1 << 1)


#define SCLK		11
#define MOSI		10
#define MISO		9
#define CS0			8
#define CS1			7


void spi_init(uint freq) {
	gpio_mode(SCLK, GPIO_FN0);
	gpio_mode(MOSI, GPIO_FN0);
	gpio_mode(MISO, GPIO_FN0);
	gpio_mode(CS0, GPIO_FN0);
	gpio_mode(CS1, GPIO_FN0);

	*SPI_CS = 0;
	*SPI_CLK = ((150000000) / freq) & 0xfffffffe;
}

void spi_begin() {
	*SPI_CS = CS_CLRFIFO | CS_TA;
}

void spi_end() {
	*SPI_CS = 0;
}

void spi_readwrite(char *buff, uint len) {
	uint i;
	for (i = 0; i < len; ++i) {
		while (!(*SPI_CS & CS_TXD));
		*SPI_FIFO = buff[i];

		while (!(*SPI_CS & CS_DONE));

		while (*SPI_CS & CS_RXD)
			buff[i] = *SPI_FIFO;
	}
}
