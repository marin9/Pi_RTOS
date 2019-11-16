.global _start
_start:
	b _init

// ############################
// interrupt vectors
// ############################

_vectors:
	ldr pc, _reset_h
    ldr pc, _undef_instr_h
    ldr pc, _soft_int_h
    ldr pc, _pref_abort_h
    ldr pc, _data_abort_h
    ldr pc, _unused_h
    ldr pc, _irq_int_h
    ldr pc, _fast_int_h

_reset_h:		.word _start
_undef_instr_h:	.word _start
_soft_int_h:	.word _start
_pref_abort_h:	.word _start
_data_abort_h:	.word _start
_unused_h:		.word _start
_irq_int_h:		.word _interrupt
_fast_int_h:	.word _start


// ############################
// cpu init
// ############################

_init:
	//system mode
 	msr cpsr_c, #0xDF

 	//set stack
	ldr sp, =_start

	//load irq vectors
  	ldr     r0, =_vectors
  	mov     r1, #0
  	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}

	bl main
	b .


// ############################
// interrupt handler
// ############################

_interrupt:
	//Load user sp
	ldr sp, =__start
	stm sp, {sp}^
	ldr sp, [sp]

	//Save context
	stmfd sp, {r0-r14}^
	sub sp, sp, #(15*4)
	push  {lr}
	mrs r0, cpsr
	mrs r1, spsr
	push  {r0-r1}

	//Change to system mode
	mov r2, sp
	msr cpsr, #0xDF
	mov sp, r2
	bl interrupt_handler

	//Restore context
	pop {r0-r1}
	mov r2, sp
	msr cpsr, r0
	msr spsr, r1
	mov sp, r2
	pop {lr}
	ldmfd sp, {r0-r14}^
	subs  pc, lr, #4
	

// ############################
// context switch
// ############################

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


// ############################
// enable interrupts
// ############################

.global interrupts_enable
interrupts_enable:
	mrs r0, cpsr
	bic r0, r0, #0x80
	msr cpsr_c, r0
	bx lr


// ############################
// disable interrupts
// ############################

.global interrupts_disable
interrupts_disable:
	mrs r0, cpsr
	orr r0, r0, #0x80
	msr cpsr_c, r0
	bx lr
