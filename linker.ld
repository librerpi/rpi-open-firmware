SECTIONS
{
  . = 0x0;
  . += SIZEOF_HEADERS;
  . = 0x200;
  _text = .;
  .text : {
    *(.text.start)
    *(.text)
    *(.text.*)
    *(.gnu.warning)
  }
  _etext = .;

  .rodata : {
    . = ALIGN(512);
    *(.rodata.vectorTable)
    *(.rodata)
    *(.rodata.*)
  }
  . = ALIGN(16);
  _erodata = .;

  . = ALIGN(32 / 8);

	PROVIDE (__init_array_start = .);
	.init_array : {
		*(.init_array)
		*(.init_array.*)
		*(.ctors)
		*(.ctors.*)
	}
	PROVIDE (__init_array_end = .);

  .data : { /* Data */
    *(.data)
  }

	.bss : {
		_edata = .;
		__bss_start = .;
		*(.bss)
	}

	. = ALIGN(32 / 8);
	_end = . ;
}
