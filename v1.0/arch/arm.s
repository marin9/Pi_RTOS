irq_vectors:
  ldr pc, _reset_h
  ldr pc, _undefined_instruction_vector_h
  ldr pc, _software_interrupt_vector_h
  ldr pc, _prefetch_abort_vector_h
  ldr pc, _data_abort_vector_h
  ldr pc, _unused_handler_h
  ldr pc, _interrupt_vector_h
  ldr pc, _fast_interrupt_vector_h

_reset_h:							              .word _start
_undefined_instruction_vector_h:	  .word _start
_software_interrupt_vector_h:     	.word _start
_prefetch_abort_vector_h:         	.word _start
_data_abort_vector_h:             	.word _start
_unused_handler_h:                	.word _start
_interrupt_vector_h:              	.word _interrupt
_fast_interrupt_vector_h:         	.word _start


_interrupt:
  // Load sp from user mod
  ldr sp, =__start
  stm sp, {sp}^
  ldr sp, [sp]

  // Save context
  stmfd sp, {r0-r14}^
  sub sp, sp, #(15*4)
  push  {lr}
  mrs r0, cpsr
  mrs r1, spsr
  push  {r0-r1}

  // Change mode
  mov r2, sp
  msr cpsr, #0xDF
  mov sp, r2
  bl  interrupt_handler

  // Restore context
  pop {r0-r1}
  mov r2, sp
  msr cpsr, r0
  msr spsr, r1
  mov sp, r2
  pop {lr}
  ldmfd sp, {r0-r14}^
  subs  pc, lr, #4


  

.global cpu_lidt;
cpu_lidt:
    push    {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9}
  	ldr     r0, =irq_vectors
  	mov     r1, #0
  	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	pop     {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9}
  	bx     lr


.global cpu_sti;
cpu_sti:
	mrs r0, cpsr
	bic r0, r0, #0x80
	msr cpsr_c, r0
	bx lr


.global cpu_cli
cpu_cli:
	mrs r0, cpsr
	orr r0, r0, #0x80
	msr cpsr_c, r0
	bx lr


.global context_switch
context_switch:
  cmp r0, #0
  beq skip
  //Save old thread state
  push {lr}
  push {sp}
  mrs r12, cpsr
  push {r0-r12}
  str sp, [r0]
skip:
  //Restore new thread state
  ldr sp, [r1]
  pop {r0-r12}
  msr cpsr_c, r12
  pop {lr, pc}
