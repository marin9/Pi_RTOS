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
}

void uart_deinit() {
	*UART0_CR = 0;
}

void uart_interrupt(uint tx, uint rx) {
	uint mask = 0;

	if (tx)
		mask |= IMSC_TXIM;

	if (rx)
		mask |= IMSC_RXIM;

	*UART0_IMSC = mask;
	*UART0_IFLS = IFSL_RX_1_8 | IFSL_TX_1_8;
}

uint uart_read(char* buff, uint len, uint block) {
	uint s = 0;
	while (s < len) {
		if (block) {
			while (*UART0_FR & FR_RXFE);
		} else {
			if (*UART0_FR & FR_RXFE)
				break;
		}
		buff[s++] = *UART0_DR;
	}
	return s;
}

uint uart_write(char* buff, uint len, uint block) {
	uint s = 0;
	while (s < len) {
		if (block) {
			while (*UART0_FR & FR_TXFF);
		} else {
			if (*UART0_FR & FR_TXFF)
				break;
		}
	 	*UART0_DR = buff[s++];
	}
	return s;
}

void uart_flush() {
	while (!(*UART0_FR & FR_TXFE));
}
