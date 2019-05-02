//Interrupt controller
#include "types.h"

#define NVIC_BASE       0x10140000
#define VICIRQSTATUS	((volatile uint*)(NVIC_BASE+0x00))
#define VICFIQSTATUS    ((volatile uint*)(NVIC_BASE+0x04))
#define VICRAWINTR      ((volatile uint*)(NVIC_BASE+0x08))
#define VICINTSELECT    ((volatile uint*)(NVIC_BASE+0x0C))
#define VICINTENABLE    ((volatile uint*)(NVIC_BASE+0x10))
#define VICINTENCLEAR	((volatile uint*)(NVIC_BASE+0x14))
#define VICSOFTINT      ((volatile uint*)(NVIC_BASE+0x18))
#define VICSOFTINTCLEAR	((volatile uint*)(NVIC_BASE+0x1C))
#define VICPROTECTION	((volatile uint*)(NVIC_BASE+0x20))

#define IRQ_COUNT 32



void irq_init(){
	*VICINTENABLE=0;	//disable all
	*VICINTSELECT=0;	//all generate irq, not fiq
	*VICSOFTINT=0;		//disable software interrupts
	*VICPROTECTION=0;	//allow user mode to control interrupts
}

int irq_get(){
	int i;
	for(i=0;i<IRQ_COUNT;++i){
		if((*VICIRQSTATUS)&(1<<i)){
			return i;
		}
	}
	return -1;
}

void irq_enable(uint irq){
	*VICINTENABLE |= (1<<irq);
}

void irq_disable(uint irq){
	 *VICINTENCLEAR=(1<<irq);
}
