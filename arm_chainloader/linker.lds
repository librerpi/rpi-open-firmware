SECTIONS
{
	. = 0x0;

	_text = .;

	.text : {
		*(.text)
		*(.text.*)
		*(.gnu.warning)
	}

	_etext = .;

	.rodata : { *(.rodata) *(.rodata.*) }
	. = ALIGN(4096);
	_erodata = .;

	. = ALIGN(32 / 8);

	PROVIDE (__init_array_start = .);
	.init_array : {
		*(.init_array)
		*(.init_array.*)
	}
	PROVIDE (__init_array_end = .);

	.data : {			/* Data */
		*(.data)
	}

	.bss : {
		_edata = .;
		__bss_start = .;
		*(.bss)
                __bss_end = .;
	}

	. = ALIGN(16);
        _irq_stack_bottom = .;
        . = . + 1024;
        _irq_stack_top = .;

        _svc_stack_bottom = .;
        . = . + 1024;
        _svc_stack_top = .;

	. = ALIGN(16);
	_end = . ;
}
