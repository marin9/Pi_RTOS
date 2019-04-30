#include "arm.h"
#include "lib.h"
#include "device.h"
#include "param.h"


extern uint __end;


void startup(){
	uart_init();
	timer_init();
	timer_set(TICK_TIME);
	mm_init(__end, HEAP_BLOCK, HEAP_NBLOCKS);

	
	irq_init();
	irq_enable(4);// umjesto 4 -> define const

	interrupts_init();
	interrupts_enable();

}

