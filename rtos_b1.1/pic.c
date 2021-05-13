#include "rpi.h"

#define IRQ_PENDING1		((volatile uint*)(IRQ_BASE + 0x204))
#define IRQ_PENDING2		((volatile uint*)(IRQ_BASE + 0x208))
#define IRQ_ENABLE1		   	((volatile uint*)(IRQ_BASE + 0x210))
#define IRQ_ENABLE2		 	((volatile uint*)(IRQ_BASE + 0x214))
#define IRQ_DISABLE1		((volatile uint*)(IRQ_BASE + 0x21C))
#define IRQ_DISABLE2		((volatile uint*)(IRQ_BASE + 0x220))


void pic_init() {
	*IRQ_DISABLE1 = 0xffffffff;
	*IRQ_DISABLE1 = 0xffffffff;
}

void pic_enable(uint irq) {
	uint n = irq % 32;
	if (irq >= 32)
		*IRQ_ENABLE2 = 1 << n;
	else
		*IRQ_ENABLE1 = 1 << n;
}

void pic_disable(uint irq) {
	uint n = irq % 32;
	if (irq >= 32)
    	*IRQ_DISABLE2 = 1 << n;
	else
    	*IRQ_DISABLE1 = 1 << n;
}

uint pic_pending(uint irq) {
	if (irq >= 32)
		return ((*IRQ_PENDING2) >> (irq - 32)) & 1;
	else
		return ((*IRQ_PENDING1) >> irq) & 1;
}
