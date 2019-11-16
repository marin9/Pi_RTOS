#include "cpu.h"
#include "device.h"

#define IRQ_COUNT	72

static void (*handler[IRQ_COUNT])();


void interrupt_init(){
	int i;
	interrupts_disable();

	for(i=0;i<IRQ_COUNT;++i){
		handler[i]=0;
	}
}

void interrupt_register(uint irq, void (*h)()){
	handler[irq]=h;
}

void interrupt_unregister(uint irq){
	handler[irq]=0;
}

void interrupt_handler(){
	int i;
	for(i=0;i<IRQ_COUNT;++i){
		if(pic_ispending(i) && handler[i]){
			handler[i]();
		}
	}
}

