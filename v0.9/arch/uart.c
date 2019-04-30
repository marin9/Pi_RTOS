#include "uart.h"

#define UART0_BASE	(0x101f1000)
#define UART0_DR	((volatile unsigned int*)(UART0_BASE+0x00))
#define UART0_FR	((volatile unsigned int*)(UART0_BASE+0x18))

void uart_init(){
	*UART0_DR;
}

int uart_send(int c){
	if((*UART0_FR)&(1<<5)){
		return 0;
	}else{
		*UART0_DR=(unsigned int)c;
		return 1;
	}
}

int uart_recv(int *c){
	if((*UART0_FR)&(1<<4)){
		return 0;
	}else{
		*c=*UART0_DR;
		return 1;
	}
}
