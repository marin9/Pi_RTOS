#include "rpi.h"
#include "types.h"

#define UART0_DR	((volatile uint*)(UART0_BASE+0x00))	//Data register
#define UART0_RSR	((volatile uint*)(UART0_BASE+0x04))	//Receive status register
#define UART0_ECR	((volatile uint*)(UART0_BASE+0x04))	//Error clear register
#define UART0_FR	((volatile uint*)(UART0_BASE+0x18))	//Flag register
#define UART0_IBRD	((volatile uint*)(UART0_BASE+0x24))	//Integer baud rate register
#define UART0_FBRD	((volatile uint*)(UART0_BASE+0x28))	//Fractional baud rate register
#define UART0_LCRH	((volatile uint*)(UART0_BASE+0x2C))	//Line control register
#define UART0_CR	((volatile uint*)(UART0_BASE+0x30))	//Control register
#define UART0_IFLS	((volatile uint*)(UART0_BASE+0x34))	//Interrupt FIFO level select register
#define UART0_IMSC	((volatile uint*)(UART0_BASE+0x38))	//Interrupt mask set/clear register
#define UART0_RIS	((volatile uint*)(UART0_BASE+0x3C))	//Raw interrupt status register
#define UART0_MIS	((volatile uint*)(UART0_BASE+0x40))	//Masked interrupt status register
#define UART0_ICR	((volatile uint*)(UART0_BASE+0x44))	//Interrupt clear register
#define UART0_DMACR	((volatile uint*)(UART0_BASE+0x48))	//DMA control register


#define GPFSEL1     ((volatile uint*)(GPIO_BASE+0x04))
#define GPPUD       ((volatile uint*)(GPIO_BASE+0x94))
#define GPPUDCLK0   ((volatile uint*)(GPIO_BASE+0x98))
#define GPPUDCLK1   ((volatile uint*)(GPIO_BASE+0x9C))


static void delay(int c){
	while(c--);
}


void uart_init(){
	// disable uart
	*UART0_CR=0;

	// set fn0 for pin 14
	*GPFSEL1=(*GPFSEL1) & ~(7<<12);
	*GPFSEL1=(*GPFSEL1) | 4<<12;

    *GPPUD=0;
    delay(200);
    *GPPUDCLK0=1<<14;
    delay(200);
    *GPPUDCLK0=0;

    // set fn0 for pin 15
    *GPFSEL1=(*GPFSEL1) & ~(7<<15);
	*GPFSEL1=(*GPFSEL1) | 4<<15;

    *GPPUD=0;
    delay(200);
    *GPPUDCLK0=1<<14;
    delay(200);
    *GPPUDCLK0=0;

    // clear all interrupt flags
    *UART0_ICR=0x7FF;

    // set baud rate to 115200
    *UART0_IBRD=27;
    *UART0_FBRD=8;

    // 8 bit word, enable fifo
    *UART0_LCRH=0x70;

    // enable rx, tx, uart
    *UART0_CR=0x301;

    // empty fifo
    int i;
    for(i=0;i<32;++i){
    	*UART0_DR;
    }
}

void uart_print(char *s){
	while(*s){
		while((*UART0_FR)&(1<<5));
		*UART0_DR=*s;
		++s;
	}
}

static int uart_send(int c){
	if((*UART0_FR)&(1<<5)){
		return -1;
	}
	*UART0_DR=(uint)c;
	return 0;
}

static int uart_recv(){
	if((*UART0_FR)&(1<<4)){
		return -1;
	}
	return *UART0_DR;
}


int uart_read(int unit, void* buffer, int size){
	int i, c;
	char *buff=(char*)buffer;

	if(unit!=0)
		return -1;

	for(i=0;i<size;++i){
		c=uart_recv();
		if(c<0)
			break;
		else
			buff[i]=(char)c;
	}
	return i;
}

int uart_write(int unit, void* buffer, int size){
	int i, c;
	char *buff=(char*)buffer;

	if(unit!=0)
		return -1;

	for(i=0;i<size;++i){
		c=uart_send(buff[i]);

		if(c<0)
			break;
	}
	return i;
}
