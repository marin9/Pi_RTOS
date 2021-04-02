#include "rpi.h"

#define BSC0_CR		((volatile uint*)(I2C_BASE + 0x00))
#define BSC0_FR		((volatile uint*)(I2C_BASE + 0x04))
#define BSC0_DLEN	((volatile uint*)(I2C_BASE + 0x08))
#define BSC0_ADDR	((volatile uint*)(I2C_BASE + 0x0C))
#define BSC0_FIFO	((volatile uint*)(I2C_BASE + 0x10))
#define BSC0_DIV	((volatile uint*)(I2C_BASE + 0x14))

#define CR_EN 		(1 << 15) // I2C enable
#define CR_INTR 	(1 << 10) // Interrupt on RXR=1
#define CR_INTT 	(1 << 9) // Interrupt on TXW=1
#define CR_INTD 	(1 << 8) // Interrupt on DONE=1
#define CR_ST 		(1 << 7) // Start transfer
#define CR_CLEAR 	(1 << 4) // Clear FIFO
#define CR_READ 	(1 << 0) // Read transfer (0-write)

#define FR_CLKT 	(1 << 9) // Clock stretch timeout
#define FR_ERR 		(1 << 8) // ACK error
#define FR_RXF 		(1 << 7) // RX FIFO full
#define FR_TXE	 	(1 << 6) // TX FIFO empty
#define FR_RXD 		(1 << 5) // RX not empty
#define FR_TXD 		(1 << 4) // TX not full
#define FR_RXR 		(1 << 3) // RX FIFO full
#define FR_TXW 		(1 << 2) // TX FIFO full
#define FR_DONE 	(1 << 1) // Transfer done
#define FR_TA 		(1 << 0) // Transfer active

#define START_READ 	(CR_EN | CR_ST | CR_CLEAR | CR_READ)
#define START_WRITE (CR_EN | CR_ST)
#define CLEAR_STAT  (FR_CLKT | FR_ERR | FR_DONE)

#define GPFSEL0     ((volatile uint*)(GPIO_BASE + 0x00))


void i2c_init(uint freq){
	*GPFSEL0 &= ~0x3f;
	*GPFSEL0 |= 0x24;
	*BSC0_DIV = (150 * 1000 * 1000) / freq;
}

int i2c_read(uint id, char *buff, uint len) {
	int i;
	int n = (len > 16) ? 16 : len;

	*BSC0_ADDR = id;
	*BSC0_DLEN = n;
	*BSC0_FR = CLEAR_STAT;
	*BSC0_CR = START_READ;
	while (!(*BSC0_FR & FR_DONE));

	for (i = 0; i < n; ++i)
		buff[i] = *BSC0_FIFO;

	return *BSC0_FR & FR_ERR;
}

int i2c_write(uint id, char *buff, uint len) {
	int i;
	int n = (len > 16) ? 16 : len;

	*BSC0_ADDR = id;
	*BSC0_DLEN = n;
	for (i = 0; i < n; ++i)
		*BSC0_FIFO = buff[i];

	*BSC0_FR = CLEAR_STAT;
	*BSC0_CR = START_WRITE;
    while (!(*BSC0_FR & FR_DONE)) {
    	asm volatile ("nop");
    }

	return *BSC0_FR & FR_ERR;
}
