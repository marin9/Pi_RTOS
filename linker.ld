ENTRY(start)
SECTIONS {
	. = 0x10000;

	.text : {
		start.o(.text*);
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
}
