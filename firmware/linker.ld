MEMORY {
  ram (rwx) : ORIGIN = 0x00000000, LENGTH = 128K
}
SECTIONS
{
  .text : {
    . = 0x0;
    . += SIZEOF_HEADERS;
    . = 0x200;
    _text = .;
    *(.text.start)
    *(.text)
    *(.text.*)
    *(.gnu.warning)
    _etext = .;
  }

  .rodata : {
    . = ALIGN(512);
    *(.rodata.vectorTable)
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(16);
    _erodata = .;
  }


  .init_array : {
    . = ALIGN(16);
    PROVIDE (__init_array_start = .);
    KEEP(*(.init_array))
    KEEP(*(.init_array.*))
    KEEP(*(.ctors))
    KEEP(*(.ctors.*))
    __init_array_end = .;
  }

  .data : { /* Data */
    *(.data)
    _edata = .;
  }

  .bss : {
    __bss_start = .;
    *(.bss)
    *(COMMON)
    . = ALIGN(32 / 8);
    _end = . ;
  }
  /* First location in stack is highest address in RAM */
  _fstack = ORIGIN(ram) + LENGTH(ram) - 4;
}
