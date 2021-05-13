#include "rpi.h"

#define UART0_DR	((volatile uint*)(UART0_BASE + 0x00))
#define UART0_RSREC	((volatile uint*)(UART0_BASE + 0x04))
#define UART0_FR	((volatile uint*)(UART0_BASE + 0x18))
#define UART0_IBRD	((volatile uint*)(UART0_BASE + 0x24))
#define UART0_FBRD	((volatile uint*)(UART0_BASE + 0x28))
#define UART0_LCRH	((volatile uint*)(UART0_BASE + 0x2C))
#define UART0_CR	((volatile uint*)(UART0_BASE + 0x30))
#define UART0_IFLS	((volatile uint*)(UART0_BASE + 0x34))
#define UART0_IMSC 	((volatile uint*)(UART0_BASE + 0x38))
#define UART0_RIS 	((volatile uint*)(UART0_BASE + 0x3C))
#define UART0_MIS 	((volatile uint*)(UART0_BASE + 0x40))
#define UART0_ICR	((volatile uint*)(UART0_BASE + 0x44))

#define FR_TXFE		(1 << 7)	// TX FIFO empty
#define FR_RXFF		(1 << 6)	// RX FIFO full
#define FR_TXFF		(1 << 5)	// TX FIFO full
#define FR_RXFE		(1 << 4)	// RX FIFO empty
#define FR_BUSY		(1 << 3)	// UART transmitting data
#define FR_CST		(1 << 0)	// Clear to send, 1 when nUARTCTS is low

#define LCRH_SPS	(1 << 7)	// Stick parity select
#define LCRH_WLEN5	(0 << 5)	// Word length 5 bits
#define LCRH_WLEN6	(1 << 5)	// Word length 6 bits
#define LCRH_WLEN7	(2 << 5)	// Word length 7 bits
#define LCRH_WLEN8	(3 << 5)	// Word length 8 bits
#define LCRH_FEN	(1 << 4)	// Enable FIFO
#define LCRH_STP2	(1 << 3)	// Two stop bit select
#define LCRH_EPS 	(1 << 2)	// Even parity select (0-odd, 1-even)
#define LCRH_PEN	(1 << 1)	// Parity enable
#define LCRH_BRK	(1 << 0)	// Send break

#define CR_CTSEN	(1 << 15)	// Clear to send enable
#define CR_RTSEN	(1 << 14)	// Request to send enable
#define CR_RTS		(1 << 11)	// Request to send
#define CR_RXEN		(1 << 9)	// Receive enable
#define CR_TXEN		(1 << 8)	// Transmit enable
#define CR_LBE		(1 << 7)	// Loopback enable
#define CR_EN		(1 << 0)	// UART enable

#define IFSL_RXIFLSEL (7 << 3) // Receive interrupt FIFO level select
#define IFSL_RX_1_8   (0 << 3) // Receive FIFO 1/8 full
#define IFSL_RX_1_4   (1 << 3) // Receive FIFO 1/4 full
#define IFSL_RX_1_2   (2 << 3) // Receive FIFO 1/2 full
#define IFSL_RX_3_4   (3 << 3) // Receive FIFO 3/4 full
#define IFSL_RX_7_8   (4 << 3) // Receive FIFO 7/8 full
#define IFSL_TXIFLSEL (7 << 0) // Transmit interrupt FIFO level select
#define IFSL_TX_1_8   (0 << 0) // Transmit FIFO 1/8 full
#define IFSL_TX_1_4   (1 << 0) // Transmit FIFO 1/4 full
#define IFSL_TX_1_2   (2 << 0) // Transmit FIFO 1/2 full
#define IFSL_TX_3_4   (3 << 0) // Transmit FIFO 3/4 full
#define IFSL_TX_7_8   (4 << 0) // Transmit FIFO 7/8 full

#define IMSC_OEIM	(1 << 10)	// Overrun error interrupt mask
#define IMSC_BEIM	(1 << 9)	// Break error interrupt mask
#define IMSC_PEIM	(1 << 8)	// Parity error interrupt mask
#define IMSC_FEIM	(1 << 7)	// Framing error interrupt mask
#define IMSC_RTIM	(1 << 6)	// Receive timeout interrupt mask
#define IMSC_TXIM	(1 << 5)	// Transmit interrupt mask
#define IMSC_RXIM	(1 << 4)	// Receive interrupt mask

#define ICR_OEIC	(1 << 10)	// Overrun error interrupt clear
#define ICR_BEIC	(1 << 9)	// Break error interrupt clear
#define ICR_PEIC	(1 << 8)	// Parity error interrupt clear
#define ICR_FEIC	(1 << 7)	// Framing error interrupt clear
#define ICR_RTIC	(1 << 6)	// Receive timeout interrupt clear
#define ICR_TXIC	(1 << 5)	// Transmit interrupt clear
#define ICR_RXIC	(1 << 4)	// Receive interrupt clear


#define UART_TX		14
#define UART_RX		15


void uart_init() {
	uint baud = 115200;
	gpio_init(UART_TX, GPIO_FN0);
	gpio_init(UART_RX, GPIO_FN0);

	*UART0_CR = 0;
	*UART0_ICR = 0x7FF;
    *UART0_IBRD = 48000000 / (16 * baud);
    *UART0_FBRD = ((*UART0_IBRD * 1000) % 1000) * 64 + 500;
    *UART0_LCRH = LCRH_FEN | LCRH_WLEN8;
    *UART0_CR = CR_EN | CR_RXEN | CR_TXEN;
}

void uart_print(char *s) {
	while (*s) {
		while (*UART0_FR & FR_TXFF);
		*UART0_DR = *s;
		if (*s == '\n') {
			while (*UART0_FR & FR_TXFF);
			*UART0_DR = '\r';
		}
		++s;
	}
}

void uart_hex(uint n) {
	uint i, hb;
	char str[8 + 1];

	for (i = 0; i < 8; ++i) {
		hb = n >> (7 - i) * 4;
		hb &= 0x0F;

		if (hb > 9)
			str[i] = (char)(hb + 'A' - 10);
		else
			str[i] = (char)(hb + '0');
	}
	str[8] = 0;
	uart_print(str);
}
/*
void uart_dec(uint n) {
	int i;
	char buff[11];

	buff[10] = 0;
	for (i = 9; n && i ; --i, n /= 10)
		buff[i] = "0123456789"[n % 10];

	uart_print(buff + i + 1);
}
*/
char uart_getchar() {
	if (*UART0_FR & FR_RXFE)
		return 0;
	else
		return *UART0_DR;
}

void uart_intren() {
	*UART0_IMSC = IMSC_RTIM;
}
