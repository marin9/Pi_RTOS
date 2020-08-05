#include "rpi.h"

static uint _sck;
static uint _si;
static uint _so;
static uint _delay_us;

#define SCK(s)	gpio_write(_sck, s)
#define SI()	gpio_read(_si)
#define SO(s)	gpio_write(_so, s)
#define DELAY()	timer_delay(_delay_us)


void sspi_init(uint sck, uint so, uint si) {
	_sck = sck;
	_si = si;
	_so = so;
	_delay_us = 1;

	gpio_mode(sck, GPIO_OUT);
	gpio_mode(si, GPIO_IN);
	gpio_mode(so, GPIO_OUT);

	SCK(0);
	SO(0);
}

void sspi_setbaud(uint br) {
	_delay_us = 1000000 / (br * 2);
}

void sspi_ssinit(uint pin) {
	gpio_mode(pin, GPIO_OUT);
	gpio_write(pin, 1);
}

void sspi_start(uint ss) {
	gpio_write(ss, 0);
	DELAY();
}

void sspi_stop(uint ss) {
	DELAY();
	gpio_write(ss, 1);
}

void sspi_send_byte(char byte) {
	int i;
	for (i = 7; i >= 0; --i) {
		SO(byte & (1 << i));
		DELAY();
		SCK(1);
		DELAY();
		SCK(0);
	}
}

char sspi_recv_byte() {
	int i;
	char byte = 0;
	for (i = 7; i >= 0; --i) {
		DELAY();
		byte |= SI() << i;
		SCK(1);
		DELAY();
		SCK(0);
	}
	return byte;
}
