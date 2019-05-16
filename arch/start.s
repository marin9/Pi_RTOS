.global _start
_start:

 	//System mode
 	mov r0, #0xDF
 	msr cpsr_c, r0
 	ldr sp, =__start

	bl startup
	b .
