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

#define DR_OE		(1 << 11) // Overrun error
#define DR_BE		(1 << 10) // Break error
#define DR_PE		(1 << 9)  // Parity error
#define DR_FE		(1 << 8)  // Framing error

#define RSREC_OE	(1 << 3)
#define RSREC_BE	(1 << 2)
#define RSREC_PE	(1 << 1)
#define RSREC_FE	(1 << 0)

#define FR_TXFE		(1 << 7)
#define FR_RXFF		(1 << 6)
#define FR_TXFF		(1 << 5)
#define FR_RXFE		(1 << 4)
#define FR_BUSY		(1 << 3)
#define FR_CST		(1 << 0)

#define LCRH_SPS	(1 << 7)
#define LCRH_WLEN5	(0 << 5)
#define LCRH_WLEN6	(1 << 5)
#define LCRH_WLEN7	(2 << 5)
#define LCRH_WLEN8	(3 << 5)
#define LCRH_FEN	(1 << 4)
#define LCRH_STP2	(1 << 3)
#define LCRH_EPS 	(1 << 2)
#define LCRH_PEN	(1 << 1)
#define LCRH_BRK	(1 << 0)

#define CR_CTSEN	(1 << 15)
#define CR_RTSEN	(1 << 14)
#define CR_RTS		(1 << 11)
#define CR_RXEN		(1 << 9)
#define CR_TXEN		(1 << 8)
#define CR_LBE		(1 << 7)
#define CR_EN		(1 << 0)

#define INT_OER 	(1 << 10)
#define INT_BER 	(1 << 9)
#define INT_PER 	(1 << 8)
#define INT_FER 	(1 << 7)
#define INT_RTR 	(1 << 6)
#define INT_TXR		(1 << 5)
#define INT_RXR		(1 << 4)

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


#define UART_RX			15
#define UART_TX			14


void uart_init(uint br) {
	int i;
	uint ibrd = 48000000 / (16 * br);
	uint fbrd = ((ibrd * 1000) % 1000) * 64 + 500;

	gpio_mode(UART_TX, GPIO_FN0);
	gpio_mode(UART_RX, GPIO_FN0);

	*UART0_CR = 0;
	*UART0_ICR = 0x7FF;
    *UART0_IBRD = ibrd;
    *UART0_FBRD = fbrd;
    *UART0_LCRH = LCRH_FEN | LCRH_WLEN8;
    *UART0_CR = CR_EN | CR_RXEN | CR_TXEN;

    for (i = 0; i < 32; ++i)
    	*UART0_DR;

    //*UART0_IMSC = INT_RXR;
}

void uart_read(char* buff, uint len) {
	uint i;
	for (i = 0; i < len; ++i) {
		while (*UART0_FR & FR_RXFE);
		buff[i] = *UART0_DR;
	}
}

void uart_write(char* buff, uint len) {
	uint i;
	for (i = 0; i < len; ++i) {
		while (*UART0_FR & FR_TXFF);
		*UART0_DR = buff[i];
	}
}

void uart_flush() {
	while (!(*UART0_FR & FR_TXFE));
}


uint uart_getc() {
	char c;
	uart_read(&c, 1);
	return c;
}

void uart_putc(char c) {
	uart_write(&c, 1);
}

void uart_print(char *s) {
	while (*s)
		uart_putc(*s++);
}
