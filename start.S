.global start
.global cpu_switch
.global cpu_interrupts

start:
 	msr cpsr_c, #0xDF
	ldr sp, =start
  	ldr     r0, =_vectors
  	mov     r1, #0
  	ldmia   r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
  	ldmia   r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
	bl setup
	b .


cpu_switch:
	push {lr}
	push {sp}
	mrs r12, cpsr
	push {r0-r12}
	str sp, [r0]
	ldr sp, [r1]
	pop {r0-r12}
	msr cpsr_c, r12
	pop {lr, pc}


cpu_interrupts:
	mrs r1, cpsr
	mrs r2, cpsr
	and r2, r2, #0x80
	cmp r0, #0
	beq enable_irq
disable_irq:
	orr r1, r1, #0x80
	msr cpsr_c, r1
	mov r0, r2
	bx lr
enable_irq:
	bic r1, r1, #0x80
	msr cpsr_c, r1
	mov r0, r2
	bx lr


_interrupt:
	ldr	sp, =start
	stm sp, {sp}^
	ldr sp, [sp]

	stmfd sp, {r0-r14}^
	sub sp, sp, #(15*4)
	push  {lr}
	mrs r0, cpsr
	mrs r1, spsr
	push {r0-r1}

	mov r2, sp
	msr cpsr, #0xDF
	mov sp, r2
	bl irq_handler

	pop {r0-r1}
	mov r2, sp
	msr cpsr, r0
	msr spsr, r1
	mov sp, r2
    pop {lr}
    ldmfd sp, {r0-r14}^
    subs pc, lr, #4


_vectors:
	ldr pc, _reset_h
    ldr pc, _undef_h
    ldr pc, _soft_h
    ldr pc, _pref_abort_h
    ldr pc, _data_abort_h
    ldr pc, _unused_h
    ldr pc, _irq_h
    ldr pc, _fast_h

_reset_h:       .word start
_undef_h:       .word start
_soft_h:        .word start
_pref_abort_h:  .word start
_data_abort_h:  .word start
_unused_h:      .word start
_irq_h:         .word _interrupt
_fast_h:        .word start
