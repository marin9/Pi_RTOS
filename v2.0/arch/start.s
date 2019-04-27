.global _start
_start:

 	//system mode
 	mov r0, #0xDF
 	msr cpsr_c, r0
 	ldr sp, =__start

	bl startup
	b .
