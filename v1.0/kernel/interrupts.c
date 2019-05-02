#include "device.h"
#include "arm.h"


void interrupt_init(){
	irq_enable(IRQ_TIMER01);
}

void interrupt_enable(){
	sti();
}

void interrupt_disable(){
	cli();
}

void interrupt_handler(){
	
	//uart_print("\tINTERRUPT\r\n");
	timer_ack();
}

void sys_entry(){


}

void sys_exit(){

	
}
