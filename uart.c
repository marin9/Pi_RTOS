#include "rpi.h"

#define UART0_DR	((volatile uint*)(UART0_BASE + 0x00))
#define UART0_FR	((volatile uint*)(UART0_BASE + 0x18))
#define UART0_IBRD	((volatile uint*)(UART0_BASE + 0x24))
#define UART0_FBRD	((volatile uint*)(UART0_BASE + 0x28))
#define UART0_LCRH	((volatile uint*)(UART0_BASE + 0x2C))
#define UART0_CR	((volatile uint*)(UART0_BASE + 0x30))
#define UART0_ICR	((volatile uint*)(UART0_BASE + 0x44))

#define CR_EN		(1 << 0)
#define CR_RXEN		(1 << 7)
#define CR_TXEN		(1 << 8)
#define FR_RXFE		(1 << 4)
#define FR_TXFF		(1 << 5)
#define FR_TXFE		(1 << 7)
#define LCRH_ENFIFO (1 << 4)
#define LCRH_WLEN8	(3 << 5)


void uart_init(uint br) {
	int i;
	uint ibrd = 48000000 / (16 * br);
	uint fbrd = ((ibrd * 1000) % 1000) * 64 + 500;
	*UART0_CR = 0;

	gpio_mode(14, GPIO_FN0);
	gpio_mode(15, GPIO_FN0);

	*UART0_ICR = 0x7FF;
    *UART0_IBRD = ibrd;
    *UART0_FBRD = fbrd;
    *UART0_LCRH = LCRH_ENFIFO | LCRH_WLEN8;
    *UART0_CR = CR_EN | CR_RXEN | CR_TXEN;

    for (i = 0; i < 32; ++i)
    	*UART0_DR;
}

uint uart_getc() {
	while (*UART0_FR & FR_RXFE);
	return *UART0_DR;
}

void uart_putc(char c) {
	while (*UART0_FR & FR_TXFF);
	*UART0_DR = c;
}

void uart_print(char *s) {
	while (*s) {
		uart_putc(*s);
		++s;
	}
}

void uart_read(char* buff, uint len) {
	uint i;
	for (i = 0; i < len; ++i)
		buff[i] = uart_getc();
}

void uart_write(char* buff, uint len) {
	uint i;
	for (i = 0; i < len; ++i)
		uart_putc(buff[i]);
}

void uart_hex(uint n) {
	int i;
	char buff[9];

	for (i = 7; i >= 0; --i) {
		buff[i] = n & 0x0f;
		n = n >> 4;
		if (buff[i] < 10)
			buff[i] += '0';
		else
			buff[i] += ('A'-10);
	}
	buff[8] = 0;
	uart_print("0x");
	uart_print(buff);
}

void uart_flush() {
	while (!(*UART0_FR & FR_TXFE));
}
