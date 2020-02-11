ENTRY(_start)

MEMORY {
  ram (rwx) : ORIGIN = 0, LENGTH = 1M
}
SECTIONS {
  .text : {
    *(.text.start)
    *(.text)
    *(.text.*)
  } >ram

  .rodata : {
    *(.todata)
  } >ram

  .data : {
    *(.data)
  } >ram

  .bss : {
    *(.bss)
  } >ram

  /* First location in stack is highest address in RAM */
  PROVIDE(_fstack = ORIGIN(ram) + LENGTH(ram) - 16);
}
