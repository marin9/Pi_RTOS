#include "cpu.h"
#include "device.h"

#define IRQ_COUNT	72

static void (*handler[IRQ_COUNT])();


void irq_init(){
	int i;
	irq_disable();

	for(i=0;i<IRQ_COUNT;++i){
		handler[i]=0;
	}
}

void irq_register(uint irq, void (*h)()){
	handler[irq]=h;
}

void irq_unregister(uint irq){
	handler[irq]=0;
}

void irq_handler(){
	int i;
	for(i=0;i<IRQ_COUNT;++i){
		if(pic_ispending(i) && handler[i]){
			handler[i]();
		}
	}
}

