.section .text.start
.global _start
_start:
  mrs x0, mpidr_el1
  and x0, x0, #3
  cbz x0, _init
0:
  wfe
  b 0b
.org 0x080
  b .
.org 0x100
  b .
.org 0x180
  b .
.org 0x200
  mrs x0, CurrentEL
  mrs x1, ELR_EL3
  mrs x2, ESR_EL3
  bl handle_exception200
  b .
.org 0x280
  b .
.org 0x300
  b .
.org 0x380
  b .
.org 0x400
  b .
.org 0x480
  b .
.org 0x500
  b .
.org 0x580
  b .
.org 0x600
  b .
.org 0x680
  b .
.org 0x700
  b .
.org 0x780
  b .

.global smc_up
smc_up:
  smc 0x1234
  b .

_init:
  mrs x0, currentel
  mov x0, #0
  msr ELR_EL1, x0
  msr ESR_EL1, x0
  msr SPSR_EL1, x0

  msr ELR_EL2, x0
  msr ESR_EL2, x0
  msr SPSR_EL2, x0

  msr ELR_EL3, x0
  msr ESR_EL3, x0
  msr SPSR_EL3, x0

  mov x5, x0
  mov x6, x0
  mov x7, x0
  mov x8, x0
  ldr x0, =_fstack
  mov sp, x0

  bl main
  b stall

stall:
  b .

.global drop_privs
drop_privs:
  msr ELR_EL3, x0
  msr SPSR_EL3, x1
  msr scr_el3, x2
  ldr x0, =_fstack
  msr SP_EL2, x0
  eret

.global reentry
reentry:
  mov x0, #0
  msr VBAR_EL2, x0
  // ldr x0, =_fstack
  // mov sp, x0
  bl el2_main
  b .
