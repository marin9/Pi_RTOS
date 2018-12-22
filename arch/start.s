.global _start
_start:

 	//system mode
 	msr cpsr_c,#0xDF
 	ldr sp, =kernel_stack

	bl startup
	b .
