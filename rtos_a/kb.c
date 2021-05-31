#include "rpi.h"

#define KB_ADDR		0x1F

#define VR		0x01	// version
#define CR		0x02	// control
#define IR		0x03	// interrupt
#define SR		0x04	// status
#define BR		0x05	// backlight
#define RR		0x08	// reset
#define DR		0x09	// fifo

#define CR_OVERFLOW_ON		(1 << 0)
#define CR_OVERFLOW_INT		(1 << 1)
#define CR_CAPSLOCK_INT		(1 << 2)
#define CR_NUMLOCK_INT		(1 << 3)
#define CR_KEY_INT			(1 << 4)
#define CR_REPORT_MODS		(1 << 6)
#define CR_USE_MODS			(1 << 7)

#define IR_INT_OVERFLOW		(1 << 0)
#define IR_INT_CAPSLOCK		(1 << 1)
#define IR_INT_NUMLOCK		(1 << 2)
#define IR_INT_KEY			(1 << 3)

#define SR_KEY_COUNT		(0x1F)
#define SR_KEY_CAPSLOCK		(1 << 5)
#define SR_KEY_NUMLOCK		(1 << 6)

#define DR_KEY_STATE		0xF0
#define DR_KEY_CODE			0x0F


// alt + Rshift = caps lock
// alt + Lshift = num lock
// any shift turns its off

int kb_init() {
	char buff[2];
	buff[0] = CR | (1 << 7);
	buff[1] = CR_USE_MODS | CR_REPORT_MODS;
	return i2c_write(KB_ADDR, buff, 2);
}

int kb_read(char *key, char *stat, char *caps) {
	char buff[2];

	buff[0] = DR;
	if (i2c_write(0x1F, buff, 1))
		return -1;

	timer_delay(10000);

	if (i2c_read(0x1F, buff, 2))
		return -2;

	*key = buff[1];
	*stat = buff[0];
	timer_delay(10000);

	buff[0] = SR;
	if (i2c_write(0x1F, buff, 1))
		return -3;

	if (i2c_read(0x1F, buff, 1))
		return -4;

	*caps = buff[0];

	return 0;
}

int kb_setbacklight(char b) {
	char buff[2];

	buff[0] = BR | (1 << 7);
	buff[1] = b;

	timer_delay(100000);
	i2c_write(KB_ADDR, buff, 2);
	timer_delay(100000);
	return 0;
}
