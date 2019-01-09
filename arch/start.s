.global _start
_start:

 	//sydtem mode
 	mov r0, #0xDF
 	msr cpsr_c, r0
 	ldr sp, =kernel_stack

	bl startup
	b .
