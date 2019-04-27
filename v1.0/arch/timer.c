#include "timer.h"
#include "irq.h"
#include "err.h"


#define TIMER0_BASE     (0x101E2000)
#define TIMER0_LOAD     ((volatile unsigned int*)(TIMER0_BASE+0x00))
#define TIMER0_VALUE    ((volatile unsigned int*)(TIMER0_BASE+0x04))
#define TIMER0_CONTROL  ((volatile unsigned int*)(TIMER0_BASE+0x08))
#define TIMER0_INTCLR   ((volatile unsigned int*)(TIMER0_BASE+0x0C))
#define TIMER0_RIS      ((volatile unsigned int*)(TIMER0_BASE+0x10))
#define TIMER0_MIS      ((volatile unsigned int*)(TIMER0_BASE+0x14))
#define TIMER0_BGLOAD   ((volatile unsigned int*)(TIMER0_BASE+0x18))
#define TIMER0_IRQ_NUMBER 4


static unsigned int last_value;
static void (*action_function)();


static void timer_handler(){
  if(action_function){
    action_function();
  }
}

static void timer_clearer(){
  *TIMER0_INTCLR=0;
}


void timer_init(){
  *TIMER0_CONTROL=0x00;
  *TIMER0_CONTROL=0xE2;
  *TIMER0_INTCLR=0;
  action_function=0;
  irq_register(TIMER0_IRQ_NUMBER, timer_handler, timer_clearer);
}

void timer_set(unsigned int usec, void (*action)()){
  action_function=action;
  last_value=usec;
  if(usec){
    *TIMER0_LOAD=usec;
  }
}

void timer_get(unsigned int *elapsed){
  ASSERT(elapsed, "Null pointer.");
  *elapsed=last_value-*TIMER0_VALUE;
}
