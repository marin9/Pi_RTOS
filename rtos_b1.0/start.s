.section .init
.global start
start:
vectors:
	ldr pc, _reset
   	ldr pc, _undef
    ldr pc, _soft
   	ldr pc, _prefabort
    ldr pc, _dataabort
    ldr pc, _unused
    ldr pc, _irq
    ldr pc, _fast

_reset:		.word reset
_undef:     .word unused
_soft:      .word svc_handler
_prefabort: .word unused
_dataabort: .word unused
_unused:    .word unused
_irq:       .word irq_handler
_fast:      .word unused

reset:
	msr cpsr_c, #0x12	// IRQ mode
	ldr sp, =0x7000

	msr cpsr_c, #0x13	// SVC mode
	ldr sp, =0x6000

 	msr cpsr_c, #0x1F	// SYS mode
	ldr sp, =0x5000

  	ldr     r0, =vectors
  	mov     r1, #0x00
  	ldmia   r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
  	ldmia   r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!, {r2, r3, r4, r5, r6, r7, r8, r9}

	bl setup
unused:
	b .
