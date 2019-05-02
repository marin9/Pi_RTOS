#include "device.h"
#include "kernel.h"
#include "prog.h"
#include "param.h"


extern uint __end;


void interrupt_handler(){
	uint irq=irq_get();
	switch(irq){
		case IRQ_TIMER01:
			time_tick();
			break;
	}
}


void startup(){
	cpu_lidt();
	uart_init();
	irq_init();
	timer_init();

	memory_init((void*)&__end, HEAP_BLOCK, HEAP_NBLOCKS);
	time_init();
	//task_init();
	//lock_init();

	cpu_sti();
	//run_main_thread
	mainprog();
}

