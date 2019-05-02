#include "types.h"
#include "device.h"
#include "param.h"

static uint ticks;


void time_init(){
	ticks=0;
	irq_enable(IRQ_TIMER01);
	timer_set(TICK_TIME);
}

void time_tick(){
	++ticks;




	//uart_print("\tINTERRUPT\r\n");
	timer_ack();

}





void time_add(time_t *ta, time_t *tb, time_t *res){
  res->sec = ta->sec + tb->sec;
	res->usec = ta->usec + tb->usec;
	if(res->usec >= 1000000){
		res->sec += 1;
		res->usec -= 1000000;
	}
}

void time_sub(time_t *ta, time_t *tb, time_t *res){
  // work only if a>b
  if(ta->usec >= tb->usec){
		res->sec = ta->sec - tb->sec;
		res->usec = ta->usec - tb->usec;
	}else{
		res->usec = ta->usec - tb->usec + 1000000;
    res->sec = ta->sec - tb->sec - 1;
	}
}

int time_cmp(time_t *ta, time_t *tb){
  if(ta->sec < tb->sec) return -1;
	if(ta->sec > tb->sec) return 1;

	if(ta->usec < tb->usec) return -1;
	if(ta->usec > tb->usec) return 1;
	return 0;
}

