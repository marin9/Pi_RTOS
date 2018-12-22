#include "uart.h"

#define UART0_BASE	(0x101f1000)
#define UART0_DR	((volatile unsigned int*)(UART0_BASE+0x00))
#define UART0_FR	((volatile unsigned int*)(UART0_BASE+0x18))

void uart_init(){
	*UART0_DR;
}

void uart_send(int c){
	while((*UART0_FR)&(1<<5));
	*UART0_DR=(unsigned int)c;
}

int uart_recv(){
	if((*UART0_FR)&(1<<4)) return 0;
	else return *UART0_DR;
}
