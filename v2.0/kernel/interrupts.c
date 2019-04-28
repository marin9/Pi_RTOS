#include "device.h"

static void uart_print(char *s){
	volatile unsigned int *DR=(unsigned int*)(0x101f1000+0x00);
	volatile unsigned int *FR=(unsigned int*)(0x101f1000+0x00);
	while(*s){
		while(((*FR)&(1<<5)));
		*DR=(unsigned int)(*s++);
	}
}


extern timer_t timer0;



void interrupt_handler(){

	uart_print("INTERRUPT\r\n");
	timer0.clri();
}

