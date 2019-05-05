#include "device.h"
#include "kernel.h"
#include "param.h"


extern uint __end;
extern void main();


void interrupt_handler(){
	uint irq=irq_get();
	switch(irq){
		case IRQ_TIMER01:
			time_tick();
			timer_ack();
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
	sem_init();
	task_init(main);

	//task_start();
	//TODO add pipe or msgq
	uart_print("ERR: task_init.\r\n");
}

