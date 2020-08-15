.global start
.global cpu_switch
.section .init

start:
	ldr pc, _reset
   	ldr pc, _undef
    ldr pc, _soft
   	ldr pc, _prefabort
    ldr pc, _dataabort
    ldr pc, _unused
    ldr pc, _irq
    ldr pc, _fast

_reset:		.word init
_undef:     .word undef
_soft:      .word soft
_prefabort: .word prefabort
_dataabort: .word dataabort
_unused:    .word 0
_irq:       .word interrupt
_fast:      .word fastinterrupt

init:
 	msr cpsr_c, #0xDF
	ldr sp, =0x8000

  	ldr     r0, =start
  	mov     r1, #0
  	ldmia   r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
  	ldmia   r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!, {r2, r3, r4, r5, r6, r7, r8, r9}

	bl main
	b .


interrupt:
	sub lr, lr, #4
	srsdb sp!, #0x1F
	cpsid if, #0x1F
	push {r0-r12}
	push {lr}
	bl irq_handler
	pop {lr}
	pop {r0-r12}
	rfeia sp!


undef:
	b .

soft:
	b .

prefabort:
	b .

dataabort:
	b .

fastinterrupt:
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
