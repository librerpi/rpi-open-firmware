/*=============================================================================
Copyright (C) 2016-2017 Authors of rpi-open-firmware
All rights reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

FILE DESCRIPTION
Entry.

A small explanation. The ROM loads bootcode.bin at 0x80000000 and jumps to
0x80000200. This region corresponds to L1/L2 cached IO and cache is never
evicted as long as we don't touch memory above that. This gives us 128KB
of memory at startup.

Exception names are from the public release from:
	brcm_usrlib\dag\vmcsx\vcfw\rtos\none\rtos_none.c

=============================================================================*/

/* main entry point */
.section .text.start
.globl _start
.align 2
_start:
  mov sp, _fstack

#if 0
  stm r0, (--sp) // dummy
  stm r0, (--sp) // values
  stm lr, (--sp)
  stm r0-r5, (--sp)
  stm r6-r15, (--sp)
  stm r16-r23, (--sp)
  mov r2, sp
#else
  mov r2, 0
#endif

  version r0
  lea r1, _start
  /* jump to C code */
  bl _main
  b .
