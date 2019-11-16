ENTRY(_start)

SECTIONS{
	. = 0x10000;
	
	__start = .;

	.text : {
		obj/arm7.o(.text*);
		*(.text*);
	}

	.rodata : {
		*(.rodata*);
	}

	.data : {		
		*(.data*);
	}

	.bss : {
		*(.bss*);
	}

	.=ALIGN(8);
	__end = .;
}