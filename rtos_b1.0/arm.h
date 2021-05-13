#pragma once

#define ARM_IRQ_MODE		0x12
#define ARM_SVC_MODE		0x13
#define ARM_SYS_MODE		0x1F
#define ARM_NO_IRQ			0x80

typedef unsigned int uint;

typedef struct {
	uint cpsr, r0, r1, r2, r3, r4, r5, r6, r7;
	uint r8, r9, r10, r11, r12, r13, lr, pc;
} context_t;

uint get_mode();
uint irq_disable();
void irq_restore(uint cpsr);
void* arm_create_context(void* stack,
						void (*thread)(void *arg),
						void* arg,
						int (*task_term)());

#define NOP()	asm volatile ("nop");
#define SVC()	asm volatile ("swi 0");

#define SAVE_CONTEXT() 							\
	asm volatile ("stmdb sp!, {r0}");			\
	asm volatile ("stmdb sp, {sp}^");			\
	asm volatile ("nop");						\
	asm volatile ("sub	sp, sp, #4");			\
	asm volatile ("ldmia sp!, {r0}");			\
	asm volatile ("stmdb r0!, {lr}");			\
	asm volatile ("mov lr, r0");				\
	asm volatile ("ldmia sp!, {r0}");			\
	asm volatile ("stmdb lr, {r0-lr}^");		\
	asm volatile ("nop");						\
	asm volatile ("sub lr, lr, #60");			\
	asm volatile ("mrs r0, spsr");				\
	asm volatile ("stmdb lr!, {r0}");			\
	asm volatile ("ldr r0, =active_thread");	\
	asm volatile ("cmp r0, #0");				\
	asm volatile ("ldrne r0, [r0]");			\
	asm volatile ("strne lr, [r0]");

#define LOAD_CONTEXT()							\
	asm volatile ("ldr r0, =active_thread");	\
	asm volatile ("ldr r0, [r0]");				\
	asm volatile ("ldr lr, [r0]");				\
	asm volatile ("ldmia lr!, {r0}");			\
	asm volatile ("msr spsr, r0");				\
	asm volatile ("ldmia lr, {r0-r14}^");		\
	asm volatile ("nop");						\
	asm volatile ("ldr lr, [lr, #60]");			\
	asm volatile ("subs pc, lr, #0");
