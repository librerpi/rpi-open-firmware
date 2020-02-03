ENTRY(_start)

MEMORY {
  ram (rwx) : ORIGIN = 0, LENGTH = 1M
}
SECTIONS {
  .text : {
    *(.text)
  } >ram
  /* First location in stack is highest address in RAM */
  PROVIDE(_fstack = ORIGIN(ram) + LENGTH(ram) - 8);
}
