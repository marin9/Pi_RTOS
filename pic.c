#include "rpi.h"

#define IRQ_PENDING_BASIC	((volatile uint*)(IRQ_BASE + 0x200))
#define IRQ_PENDING1		((volatile uint*)(IRQ_BASE + 0x204))
#define IRQ_PENDING2		((volatile uint*)(IRQ_BASE + 0x208))
#define FIRQ_CONTROL		((volatile uint*)(IRQ_BASE + 0x20C))
#define IRQ_ENABLE1		   	((volatile uint*)(IRQ_BASE + 0x210))
#define IRQ_ENABLE2		 	((volatile uint*)(IRQ_BASE + 0x214))
#define IRQ_ENABLE_BASIC	((volatile uint*)(IRQ_BASE + 0x218))
#define IRQ_DISABLE1		((volatile uint*)(IRQ_BASE + 0x21C))
#define IRQ_DISABLE2		((volatile uint*)(IRQ_BASE + 0x220))
#define IRQ_DISABLE_BASIC	((volatile uint*)(IRQ_BASE + 0x224))


static void (*irq_handlers[IRQ_COUNT])();


static uint pic_ispending(uint irqn) {
	if (irqn >= 32)
		return ((*IRQ_PENDING2) >> (irqn - 32)) & 1;
	else
		return ((*IRQ_PENDING1) >> irqn) & 1;
}

void irq_handler() {
	int i;

	for (i = 0; i < IRQ_COUNT; ++i)
		if (pic_ispending(i) && irq_handlers[i])
			irq_handlers[i]();
}


void pic_init() {
	int i;

	*IRQ_DISABLE1 = 0xffffffff;
	*IRQ_DISABLE1 = 0xffffffff;
	*IRQ_DISABLE_BASIC = 0xffffffff;

	for (i = 0; i < IRQ_COUNT; ++i)
		irq_handlers[i] = 0;
}

void pic_register(uint irqn, void (*handler)()) {
	uint n = irqn % 32;

	irq_handlers[irqn] = handler;

	if (irqn >= 32)
		*IRQ_ENABLE2 = 1 << n;
	else
		*IRQ_ENABLE1 = 1 << n;
}

void pic_unregister(uint irqn) {
	uint n = irqn % 32;

	irq_handlers[irqn] = 0;

	if (irqn >= 32)
    	*IRQ_DISABLE2 = 1 << n;
	else
    	*IRQ_DISABLE1 = 1 << n;
}
