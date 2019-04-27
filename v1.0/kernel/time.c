#include "time.h"
#include "timer.h"
#include "task.h"
#include "err.h"

#define SCHED_PERIOD_US  1000

static time_t system_clock;
static time_t sched_period;


static void timer_handler(){
  time_add(&system_clock, &sched_period, &system_clock);
  task_scheduler();
}


void time_init(){
  system_clock.sec=0;
  system_clock.usec=0;
  sched_period.sec=0;
  sched_period.usec=SCHED_PERIOD_US;
  timer_init();
  timer_set(SCHED_PERIOD_US, timer_handler);
}

void time_set(time_t *newtime){
  ASSERT(newtime, "Null pointer.");
  system_clock.sec=newtime->sec;
  system_clock.usec=newtime->usec;
  timer_set(SCHED_PERIOD_US, timer_handler);
}

void time_get(time_t *current){
  ASSERT(current, "Null pointer.");
  time_t elapsed;
  elapsed.sec=0;
  timer_get(&(elapsed.usec));
  time_add(&system_clock, &elapsed, current);
}



void time_add(time_t *ta, time_t *tb, time_t *res){
  ASSERT(ta && tb && res, "Null pointer.");
  res->sec = ta->sec + tb->sec;
	res->usec = ta->usec + tb->usec;
	if(res->usec >= 1000000){
		res->sec += 1;
		res->usec -= 1000000;
	}
}

void time_sub(time_t *ta, time_t *tb, time_t *res){
  // work only if a>b
  ASSERT(ta && tb && res, "Null pointer.");
  if(ta->usec >= tb->usec){
		res->sec = ta->sec - tb->sec;
		res->usec = ta->usec - tb->usec;
	}else{
		res->usec = ta->usec - tb->usec + 1000000;
    res->sec = ta->sec - tb->sec - 1;
	}
}

int time_cmp(time_t *ta, time_t *tb){
  ASSERT(ta && tb, "Null pointer.");
  if(ta->sec < tb->sec) return -1;
	if(ta->sec > tb->sec) return 1;

	if(ta->usec < tb->usec) return -1;
	if(ta->usec > tb->usec) return 1;
	return 0;
}