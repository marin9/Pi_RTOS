ENTRY(_start)
KSTACK_SIZE = 0x1000;
HEAP_SIZE = 0x4000000;

SECTIONS{
	. = 0x10000;

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

	. = ALIGN(4);
	. = . + KSTACK_SIZE;
	kernel_stack = . ;

	. = ALIGN(4);
	heap_start = . ;
	. = . + HEAP_SIZE;
	__end = .;
}
