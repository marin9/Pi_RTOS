#include "rpi.h"

#define BSC0_CR		((volatile uint*)(I2C_BASE + 0x00))
#define BSC0_FR		((volatile uint*)(I2C_BASE + 0x04))
#define BSC0_DLEN	((volatile uint*)(I2C_BASE + 0x08))
#define BSC0_ADDR	((volatile uint*)(I2C_BASE + 0x0C))
#define BSC0_FIFO	((volatile uint*)(I2C_BASE + 0x10))
#define BSC0_DIV	((volatile uint*)(I2C_BASE + 0x14))

#define CR_EN 		(1 << 15)
#define CR_INTR 	(1 << 10)
#define CR_INTT 	(1 << 9)
#define CR_INTD 	(1 << 8)
#define CR_ST 		(1 << 7)
#define CR_CLEAR 	(1 << 4)
#define CR_READ 	(1 << 0)

#define FR_CLKT 	(1 << 9)
#define FR_ERR 		(1 << 8)
#define FR_RXF 		(1 << 7)
#define FR_TXE	 	(1 << 6)
#define FR_RXD 		(1 << 5)
#define FR_TXD 		(1 << 4)
#define FR_RXR 		(1 << 3)
#define FR_TXW 		(1 << 2)
#define FR_DONE 	(1 << 1)
#define FR_TA 		(1 << 0)

#define START_READ 	(CR_EN | CR_ST | CR_CLEAR | CR_READ)
#define START_WRITE (CR_EN | CR_ST)
#define CLEAR_STAT  (FR_CLKT | FR_ERR | FR_DONE)

#define GPFSEL0     ((volatile uint*)(GPIO_BASE + 0x00))


static void delay(uint us) {
	us += timer_get();
	while (timer_get() < us)
		asm volatile("nop");
}

static int wait_i2c_done() {
	int timeout = 50;

	while (!(*BSC0_FR & FR_DONE) && --timeout)
		delay(1000);

	if(!timeout)
		return -1;
	else
		return 0;
}


void i2c_init(){
	*GPFSEL0 &= ~0x3f;
	*GPFSEL0 |= 0x24;
	//*BSC0_DIV = 0x100;
}

int i2c_read(uint id, char *buff, uint len) {
	int i;
	int n = (len > 16) ? 16 : len;

	*BSC0_ADDR = id;
	*BSC0_DLEN = n;
	*BSC0_FR = CLEAR_STAT;
	*BSC0_CR = START_READ;
	if (wait_i2c_done())
		return -1;

	for (i = 0; i < n; ++i)
		buff[i] = *BSC0_FIFO;

	return n;
}

int i2c_write(uint id, char *buff, uint len) {
	int i;
	int n = (len > 16) ? 16 : len;

	*BSC0_ADDR = id;
	*BSC0_DLEN = n;
	for (i = 0; i < 16; ++i)
		*BSC0_FIFO = buff[i];

	*BSC0_FR = CLEAR_STAT;
	*BSC0_CR = START_WRITE;
    if (wait_i2c_done())
		return -1;

	return n;
}


/*
24LC128 memory

static void read(uint addr, char *buff, uint len) {
	uint x = 0;
	char cmd[2];

	cmd[0] = (addr >> 8) & 0xff;
	cmd[1] = addr & 0xff;
	i2c_write(0x50, cmd, 2);

	while (x < len) {
		x += i2c_read(0x50, buff + x, len - x);
	}
}


static void write_block(uint addr, char *buff) {
	char block[16];

	block[0] = (addr >> 8) & 0xff;
	block[1] = addr & 0xff;
	memcpy(block + 2, buff, 14);

	i2c_write(0x50, block, 16);
	delay(10000);
}

static void write(uint addr, char *buff, uint len) {
	uint x = 0;
	uint adr = addr;

	while (x < len) {
		write_block(adr, buff + x);
		x += 14;
		adr += 14;
	}
}

*/
