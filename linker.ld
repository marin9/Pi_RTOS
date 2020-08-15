ENTRY(start)
SECTIONS {
	. = 0x10000;

	.init : {
		*(.init*)
	}

	.text : {
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
