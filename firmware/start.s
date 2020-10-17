# 1 "start.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "start.S"
# 29 "start.S"
.section .text.start
.globl _start
.align 2
_start:
  mov sp, _fstack
# 44 "start.S"
  mov r2, 0


  version r0
  lea r1, _start

  bl _main
  b .

findme:
  mov r0, sr
  rts
