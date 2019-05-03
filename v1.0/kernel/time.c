#include "types.h"
#include "device.h"
#include "param.h"

static uint system_time_us;


void time_init(){
	system_time_us=0;
	irq_enable(IRQ_TIMER01);
	timer_set(TICK_TIME);
}

void time_tick(){
	++system_time_us;
	timer_ack();
}

uint time_get(){
	return system_time_us;
}
