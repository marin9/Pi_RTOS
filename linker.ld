ENTRY(start)
SECTIONS {
	. = 0x8000;

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
