#include "types.h"
#include "device.h"

#define TIMER0_BASE     0x101E2000

#define TIMER0_LOAD     0x00
#define TIMER0_VALUE    0x04
#define TIMER0_CONTROL  0x08
#define TIMER0_INTCLR   0x0C
#define TIMER0_RIS      0x10
#define TIMER0_MIS      0x14
#define TIMER0_BGLOAD   0x18


//Timer controll register
#define TMR_EN   (1<<7) //Enable timer (counting)
#define TMR_MP   (1<<6) //Periodic
#define TMR_IEN  (1<<5) //Enable timer interrupt
//bit 4 - reserved
//bits 3-2: prescale (divisor): 0-1;1-16;2->256;3-not in use
#define TMR_S32  (1<<1)  //32-bit
#define TMR_1S   (1<<0)  //One shot


static volatile uint *timer_base=(uint*)TIMER0_BASE;
static uint VALUE;


static void timer_init(){
  *(timer_base+TIMER0_CONTROL)=0x00;
  *(timer_base+TIMER0_CONTROL)=TMR_EN|TMR_MP|TMR_IEN|TMR_S32;
  *(timer_base+TIMER0_LOAD)=VALUE=-1;
  *(timer_base+TIMER0_INTCLR)=0;
}

static void timer_clri(){
  *(timer_base+TIMER0_INTCLR)=0;
}

static uint timer_get(){
  return VALUE-*(timer_base+TIMER0_VALUE);
}

static void timer_set(uint us){
	*(timer_base+TIMER0_LOAD)=VALUE=us;
	*(timer_base+TIMER0_INTCLR)=0;
}

static void timer_destroy(){
	*(timer_base+TIMER0_CONTROL)=0x00;
	*(timer_base+TIMER0_INTCLR)=0;
}


timer_t timer0=(timer_t){
	.dev_id=0,
	.irq_num=4,
	.timer_init=timer_init,
	.timer_clri=timer_clri,
	.timer_get=timer_get,
	.timer_set=timer_set,
	.timer_destroy=timer_destroy
};
