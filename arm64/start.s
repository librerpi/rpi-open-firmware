.text
.global _start
_start:
  b .
.global test
test:
  ldr x0, =_fstack
  mov sp, x0

  mrs x0, mpidr_el1
  and x0, x0, #3
  cbz x0, _init
0:
  wfe
  b 0b

_init:
  bl main
  b _start
uart_data:
  .word 0x3f201000
u:
  .byte 'U'
