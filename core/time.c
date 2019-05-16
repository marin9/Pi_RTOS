#include "types.h"
#include "param.h"
#include "core.h"
#include "dev.h"
#include "lib.h"


static uint system_time_ms;
static queue_t sleep_tasks;


void time_init(){
	system_time_ms=0;
	queue_init(&sleep_tasks);
	irq_enable(IRQ_TIMER01);
	timer_set(TICK_TIME);
}

void time_tick(){
	++system_time_ms;
	task_wakeup_all(&sleep_tasks);
	task_sched();
}

uint time_get(){
	return system_time_ms;
}

void time_wait(uint ms){
	uint wakeup_time=time_get()+ms;
	while(wakeup_time>time_get()){
		task_wait(&sleep_tasks);
	}
}
