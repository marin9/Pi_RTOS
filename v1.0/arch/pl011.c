#include "types.h"

#define UART0_BASE	0x101f1000

#define UART_DR		((volatile uint*)(UART0_BASE+0x00))	//Data register
#define UART_RSR	((volatile uint*)(UART0_BASE+0x04))	//Receive status register
#define UART_ECR	((volatile uint*)(UART0_BASE+0x04))	//Error clear register
#define UART_FR		((volatile uint*)(UART0_BASE+0x18))	//Flag register
#define UART_IBRD	((volatile uint*)(UART0_BASE+0x24))	//Integer baud rate register
#define UART_FBRD	((volatile uint*)(UART0_BASE+0x28))	//Fractional baud rate register
#define UART_LCR_H	((volatile uint*)(UART0_BASE+0x2C))	//Line control register
#define UART_CR		((volatile uint*)(UART0_BASE+0x30))	//Control register
#define UART_IFLS	((volatile uint*)(UART0_BASE+0x34))	//Interrupt FIFO level select register
#define UART_IMSC	((volatile uint*)(UART0_BASE+0x38))	//Interrupt mask set/clear register
#define UART_RIS	((volatile uint*)(UART0_BASE+0x3C))	//Raw interrupt status register
#define UART_MIS	((volatile uint*)(UART0_BASE+0x40))	//Masked interrupt status register
#define UART_ICR	((volatile uint*)(UART0_BASE+0x44))	//Interrupt clear register
#define UART_DMACR	((volatile uint*)(UART0_BASE+0x48))	//DMA control register


#define DR_ERR_MASK		0x0f00
#define UART_HZ			24000000	//uart input frequency (assuming 24 MHz)
#define UART_BIT_RATE	115200		//desired bit rate
#define UART_IBRD_V		(UART_HZ/(16*UART_BIT_RATE))
#define UART_FBRD_V		((uint)((1. *UART_HZ/(16. *UART_BIT_RATE)-UART_IBRD_V)*64+0.5))
#define UART_LCR_H_WL	0x60	//8 bit word
#define UART_LCR_H_FEN	0x10	//Enable FIFO
#define UART_CR_RXE		0x20	//Receive enable
#define UART_CR_TXE		0x10	//Transmit enable
#define UART_CR_UARTEN	0x01	//UART enable
#define UART_IMSC_TXIM	0x20	//Transmit interrupt
#define UART_IMSC_RXIM	0x10	//Receive interrupt
#define UART_FR_TXFF	(1<<5)	//Transmit FIFO full
#define UART_FR_RXFE	(1<<4)	//Receive FIFO empty


void uart_init(){
	*UART_CR=UART_CR_UARTEN;
}

int uart_send(int c){
	if((*UART_FR)&(1<<5)){
		return 0;
	}else{
		*UART_DR=(uint)c;
		return 1;
	}
}

int uart_recv(int *c){
	if((*UART_FR)&(1<<4)){
		return 0;
	}else{
		*c=*UART_DR;
		return 1;
	}
}

void uart_putc(int c){
	while(!uart_send(c));
}

uint uart_getc(){
	int c;
	while(!uart_recv(&c));
	return c;
}

void uart_print(char *s){
	while(*s){
		uart_putc(*s);
		++s;
	}
}

void uart_printhex(uint x){
	uint d, s, m;
	uart_print("0x");

	s=28;
	m=0xf0000000;
	do{
		d=(x&m)>>s;
		if(d>9) uart_putc('A'+d-10);
		else uart_putc('0'+d);
		m=(m>>4);
		s=(s-4);
	}while(m);
}

uint uart_gets(char *b, uint n){
	uint i;
	for(i=0;i<n;++i){
		b[i]=uart_getc();
		if(b[i]=='\r'){
			b[i]=0;
			break;
		}
		uart_putc(b[i]);
	}
	uart_print("\r\n");
	return i;
}
