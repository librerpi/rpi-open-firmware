.global _start
_start:
  mov r0, 0x1000000
loop2:
  addcmpbne r0, -1, 0x0, loop2
  bl main_entry
loop:
  b loop
