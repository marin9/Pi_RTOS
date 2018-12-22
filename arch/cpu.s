irq_vectors:
		    ldr pc, _reset_h
        ldr pc, _undefined_instruction_vector_h
        ldr pc, _software_interrupt_vector_h
        ldr pc, _prefetch_abort_vector_h
        ldr pc, _data_abort_vector_h
        ldr pc, _unused_handler_h
        ldr pc, _interrupt_vector_h
        ldr pc, _fast_interrupt_vector_h

_reset_h:														.word _reset
_undefined_instruction_vector_h:		.word _undef_instruction
_software_interrupt_vector_h:       .word _software_interrupt
_prefetch_abort_vector_h:           .word _prefetch_abort
_data_abort_vector_h:               .word _data_abort
_unused_handler_h:                  .word _not_used
_interrupt_vector_h:                .word _interrupt
_fast_interrupt_vector_h:           .word _fiq


.global cpu_init;
cpu_init:
 	push    {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9}
	// copy irq vectors to address 0
  	ldr     r0, =irq_vectors
  	mov     r1, #0
  	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	pop     {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9}
  	bx     lr


.global cpu_suspend
cpu_suspend:
	bx lr


.global cpu_halt
cpu_halt:
	 bl disable_interrupts
	 b .


.global enable_interrupts
enable_interrupts:
	push {r0}
	mrs r0,cpsr
	bic r0,r0,#0x80
	msr cpsr_c,r0
	pop {r0}
	bx lr


.global disable_interrupts
disable_interrupts:
	push {r0}
	mrs r0,cpsr
	orr r0,r0,#0x80
	msr cpsr_c,r0
	pop {r0}
	bx lr



.global context_switch
context_switch:
	// Save old thread state
	cmp r0, #0
	beq skip
	push {lr}
	push {sp}
	mrs r12, cpsr
	push {r0-r12}
	str sp, [r0]
skip:
	// Restore new thread state
	ldr sp, [r1]
	pop {r0-r12}
	msr cpsr_c, r12
	pop {lr, pc}




_reset:
	b _start
	b .

_undef_instruction:
	ldr sp, =kernel_stack
	bl undefined_instruction
	bl .

_software_interrupt:
	ldr sp, =kernel_stack
	bl software_interrupt
	bl .

_prefetch_abort:
	ldr sp, =kernel_stack
	bl prefetch_abort
	b .

_data_abort:
	ldr sp, =kernel_stack
	bl data_abort
	b .

_not_used:
	b .

_fiq:
	ldr sp, =kernel_stack
	bl fast_interrupt_handler
	b .



irq_stack:		.word	0
_irq_stack_:	.word	irq_stack + 4


_interrupt:
	// Load sp from user mod
	ldr sp, _irq_stack_
	stmfd sp, {sp}^
	ldr sp, [sp, #-4]
	// save context
	stmfd	sp, {r0-r14}^
	sub	sp, sp, #(15*4)
	push	{lr}
	mrs	r0, cpsr
	mrs	r1, spsr
	push	{r0-r1}
	// save sp while change mode
	mov	r2, sp
	msr	cpsr, #0xdf
	mov	sp, r2
	bl	interrupt_handler
	// restore context
	pop	{r0-r1}
	mov	r2, sp
	msr	cpsr, r0
	msr	spsr, r1
	mov	sp, r2
	pop	{lr}
	ldmfd	sp, {r0-r14}^
	subs	pc, lr, #4
