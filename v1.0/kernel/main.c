#include "device.h"
#include "kernel.h"
#include "arm.h"
#include "prog.h"
#include "param.h"


extern uint __end;


void startup(){
	lidt();
	uart_init();
	irq_init();
	timer_init();

	memory_init((void*)&__end, HEAP_BLOCK, HEAP_NBLOCKS);
	interrupt_init();
	interrupt_enable();

	mainprog();
}

