#pragma once
#include "types.h"

#define CPSR_MODE_FIQ	0x11
#define CPSR_MODE_IRQ	0x12
#define CPSR_MODE_SVC	0x13
#define CPSR_MODE_ABT	0x17
#define CPSR_MODE_UND	0x1b
#define CPSR_MODE_SYS	0x1f

#define CPSR_FLAG_T		0x20
#define CPSR_FLAG_F		0x40
#define CPSR_FLAG_I		0x80


typedef struct{
	uint r0;
	uint r1;
	uint r2;
	uint r3;
	uint r4;
	uint r5;
	uint r6;
	uint r7;
	uint r8;
	uint r9;
	uint r10;
	uint r11;
	uint cpsr;
	uint lr;
	uint pc;
} context_t;

void interrupts_enable();
void interrupts_disable();
void context_switch(void *c1, void *c2);
void context_create(void *ctx, void *fn, void *reap);
