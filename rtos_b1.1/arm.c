#include "arm.h"

extern void rtos_tick();
extern void rtos_sched();
extern void irq_user();


void __attribute__((naked)) irq_handler() {
	asm volatile ("sub lr, lr, #4");
	SAVE_CONTEXT();
	rtos_tick();
	irq_user();
	LOAD_CONTEXT();
}

void __attribute__((naked)) svc_handler() {
	SAVE_CONTEXT();
	rtos_sched();
	LOAD_CONTEXT();
}


uint get_mode() {
	uint mode;
	asm volatile ("mrs %0, cpsr" : "=r" (mode));
	mode &= 0x1F;
	return mode;
}

uint irq_disable() {
	asm volatile ("mrs r0, cpsr");
	asm volatile ("mov r1, r0");
	asm volatile ("orr r1, r1, #0xC0");
	asm volatile ("msr cpsr, r1");
	asm volatile ("bx lr");
	return 0;
}

void irq_restore(uint cpsr) {
	asm volatile ("msr cpsr, %0" : : "r" (cpsr));
}

void* arm_create_context(void* stack,
						void (*thread)(void *arg),
						void* arg,
						void (*task_term)()) {
	void *sp = (uint*)(stack - sizeof(context_t));
	context_t *ctx = (context_t*)sp;

	ctx->cpsr = ARM_SYS_MODE;
	ctx->r0 = (uint)arg;
	ctx->r1 = 0x01010101;
	ctx->r2 = 0x02020202;
	ctx->r3 = 0x03030303;
	ctx->r4 = 0x04040404;
	ctx->r5 = 0x05050505;
	ctx->r6 = 0x06060606;
	ctx->r7 = 0x07070707;
	ctx->r8 = 0x08080808;
	ctx->r9 = 0x09090909;
	ctx->r10 = 0x0A0A0A0A;
	ctx->r11 = 0x0B0B0B0B;
	ctx->r12 = 0x0C0C0C0C;
	ctx->r13 = (uint)stack;
	ctx->lr = (uint)task_term;
	ctx->pc = (uint)thread;
	return sp;
}
