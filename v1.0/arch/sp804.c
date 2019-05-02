//Timer
#include "types.h"

#define TIMER0_BASE     0x101E2000
#define TIMER0_LOAD     ((volatile uint*)(TIMER0_BASE+0x00))
#define TIMER0_VALUE    ((volatile uint*)(TIMER0_BASE+0x04))
#define TIMER0_CONTROL  ((volatile uint*)(TIMER0_BASE+0x08))
#define TIMER0_INTCLR   ((volatile uint*)(TIMER0_BASE+0x0C))
#define TIMER0_RIS      ((volatile uint*)(TIMER0_BASE+0x10))
#define TIMER0_MIS      ((volatile uint*)(TIMER0_BASE+0x14))
#define TIMER0_BGLOAD   ((volatile uint*)(TIMER0_BASE+0x18))


//Timer controll register
#define TMR_EN   (1<<7) //Enable timer (counting)
#define TMR_MP   (1<<6) //Periodic
#define TMR_IEN  (1<<5) //Enable timer interrupt
//bit 4 - reserved
//bits 3-2: prescale (divisor): 0-1;1-16;2->256;3-not in use
#define TMR_S32  (1<<1)  //32-bit
#define TMR_1S   (1<<0)  //One shot


static uint VALUE;


void timer_init(){
  *TIMER0_CONTROL=0x00;
  *TIMER0_CONTROL=TMR_EN|TMR_MP|TMR_IEN|TMR_S32;
  *TIMER0_INTCLR=0;
}

void timer_ack(){
  	*TIMER0_INTCLR=0;
}

uint timer_get(){
  return VALUE-*TIMER0_VALUE;
}

void timer_set(uint us){
	*TIMER0_INTCLR=0;
	*TIMER0_LOAD=us;
	VALUE=us;
}

void timer_destroy(){
	*TIMER0_CONTROL=0x00;
	*TIMER0_INTCLR=0;
}
