irq_vectors:
  ldr pc, _reset_h
  ldr pc, _undefined_instruction_vector_h
  ldr pc, _software_interrupt_vector_h
  ldr pc, _prefetch_abort_vector_h
  ldr pc, _data_abort_vector_h
  ldr pc, _unused_handler_h
  ldr pc, _interrupt_vector_h
  ldr pc, _fast_interrupt_vector_h

_reset_h:							.word _start
_undefined_instruction_vector_h:	.word _not_used
_software_interrupt_vector_h:     	.word _not_used
_prefetch_abort_vector_h:         	.word _not_used
_data_abort_vector_h:             	.word _not_used
_unused_handler_h:                	.word _not_used
_interrupt_vector_h:              	.word _interrupt
_fast_interrupt_vector_h:         	.word _not_used


_interrupt:
  // Load sp from user mod
  ldr sp, irq_stack_
  stmfd sp, {sp}^
  ldr sp, [sp, #-4]
  // save context
  stmfd sp, {r0-r14}^
  sub sp, sp, #(15*4)
  push  {lr}
  mrs r0, cpsr
  mrs r1, spsr
  push  {r0-r1}
  // save sp while change mode
  mov r2, sp
  msr cpsr, #0xdf
  mov sp, r2
  bl  interrupt_handler
  // restore context
  pop {r0-r1}
  mov r2, sp
  msr cpsr, r0
  msr spsr, r1
  mov sp, r2
  pop {lr}
  ldmfd sp, {r0-r14}^
  subs  pc, lr, #4
irq_stack:
  .word 0
irq_stack_:
  .word irq_stack+4


_not_used:
  b .
  


.global interrupts_init;
interrupts_init:
    push    {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9}
  	ldr     r0, =irq_vectors
  	mov     r1, #0
  	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	pop     {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9}
  	bx     lr


.global interrupts_enable
interrupts_enable:
	mrs r0, cpsr
	bic r0, r0, #0x80
	msr cpsr_c, r0
	bx lr


.global interrupts_disable
interrupts_disable:
	mrs r0, cpsr
	orr r0, r0, #0x80
	msr cpsr_c, r0
	bx lr
