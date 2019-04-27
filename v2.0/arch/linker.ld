ENTRY(_start)
SECTIONS{
	. = 0x10000;
	__start = .;

	.text : {
		obj/start.o(.text*);
		*(.text*);
	}

	.data : {
		*(.rodata*);
		*(.data*);
	}

	.bss : {
		*(.bss*);
	}

	. = ALIGN(8);
	__end = .;
}
