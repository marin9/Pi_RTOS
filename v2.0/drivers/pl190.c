#include "types.h"
#include "device.h"

#define NVIC_BASE       0x10140000

#define VICIRQSTATUS	0x00
#define VICFIQSTATUS    0x04
#define VICRAWINTR      0x08
#define VICINTSELECT    0x0C
#define VICINTENABLE    0x10
#define VICINTENCLEAR	0x14
#define VICSOFTINT      0x18
#define VICSOFTINTCLEAR	0x01C
#define VICPROTECTION	0x020

#define IRQ_COUNT 32


static volatile uint *intrcon_base=(uint*)NVIC_BASE;



static void irq_init(){
	*(intrcon_base+VICINTENABLE)=0;	//disable all
	*(intrcon_base+VICINTSELECT)=0;	//all generate irq, not fiq
	*(intrcon_base+VICSOFTINT)=0;	//disable software interrupts
	*(intrcon_base+VICPROTECTION)=0;//allow user mode to control interrupts
}

static int irq_get(){
	int i;
	for(i=0;i<IRQ_COUNT;++i){
		if(*(intrcon_base+VICIRQSTATUS)&(1<<i)){
			return i;
		}
	}
	return -1;
}

static void irq_enable(uint irq){
	*(intrcon_base+VICINTENABLE) |= (1<<irq);
}

static void irq_disable(uint irq){
	 *(intrcon_base+VICINTENCLEAR) = (1<<irq);
}


irqcontroller_t irqcon=(irqcontroller_t){
	.irq_init=irq_init,
	.irq_get=irq_get,
	.irq_enable=irq_enable,
	.irq_disable=irq_disable
};
