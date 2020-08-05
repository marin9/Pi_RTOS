#include "rpi.h"
#include "dev.h"

static uint rs, rw, en;
static uint d7, d6, d5, d4;

#define RS(s) gpio_write(rs, s);
#define RW(s) gpio_write(rw, s);
#define EN(s) gpio_write(en, s);
#define D7(s) gpio_write(d7, s);
#define D6(s) gpio_write(d6, s);
#define D5(s) gpio_write(d5, s);
#define D4(s) gpio_write(d4, s);
#define WAIT(t) timer_delay(t);


static void lcd1602_write(char byte, char data) {
	RS(data);
	RW(0);
	EN(1);
	D7(byte & 0x80);
	D6(byte & 0x40);
	D5(byte & 0x20);
	D4(byte & 0x10);
	WAIT(1);

	EN(0);
	WAIT(1);

	EN(1);
	D7(byte & 0x08);
	D6(byte & 0x04);
	D5(byte & 0x02);
	D4(byte & 0x01);
	WAIT(1);

	EN(0);
	WAIT(50);
}


void lcd1602_init(uint p_rs, uint p_rw, uint p_en,
					uint p_d7, uint p_d6, uint p_d5, 
					uint p_d4) {
	rs = p_rs;
	rw = p_rw;
	en = p_en;
	d7 = p_d7;
	d6 = p_d6;
	d5 = p_d5;
	d4 = p_d4;

	gpio_mode(rs, GPIO_OUT);
	gpio_mode(rw, GPIO_OUT);
	gpio_mode(en, GPIO_OUT);
	gpio_mode(d7, GPIO_OUT);
	gpio_mode(d6, GPIO_OUT);
	gpio_mode(d5, GPIO_OUT);
	gpio_mode(d4, GPIO_OUT);
	gpio_write(en, 0);

	lcd1602_clear();
}

void lcd1602_control(char on, char cursor, char blink) {
	char cmd = 0x08;
	if (on)
		cmd |= (1 << 2);
	if (cursor)
		cmd |= (1 << 1);
	if (blink)
		cmd |= (1 << 0);

	lcd1602_write(cmd, 0);
}

void lcd1602_cgram_addr(char adr) {
	lcd1602_write(0x40 | (adr & 0x3f), 0);
}

void lcd1602_ddram_addr(char adr) {
	lcd1602_write(0x80 | (adr & 0x7f), 0);
}

void lcd1602_ramwrite(char c) {
	lcd1602_write(c, 1);
}

void lcd1602_clear() {
	lcd1602_write(0x01, 0);
	WAIT(2000);
	lcd1602_write(0x02, 0);
	WAIT(2000);	
}

void lcd1602_putc(char c) {
	lcd1602_write(c, 1);
}

void lcd1602_print(char *s) {
	while (*s)
		lcd1602_putc(*s++);
}

void lcd1602_setcursor(char line, char col) {
	if (line == 0)
		lcd1602_ddram_addr(col);
	else if (line ==1)
		lcd1602_ddram_addr(64 + col);
}
