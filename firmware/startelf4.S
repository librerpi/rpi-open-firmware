.global _start
_start:
  mov r0, 0x1000000
loop2:
  addcmpbne r0, -1, 0x0, loop2
  di
  bl main_entry
loop:
  b loop

.global gpio_test
gpio_test:
  nop
  mov r2, 0x7e003004
gpio_loop:
  ld r3, (r2)
  st r3, (r1)
  add r1, 0x4
  addcmpbne r0, -1, 0x0, gpio_loop
  rts
