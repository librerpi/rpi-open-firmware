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

.text

/* MMIO-mapped registers for the interrupt table */

.set IC0_BASE, 0x7e002000
.set IC0_VADDR, 0x7e002030

.set IC1_BASE, 0x7e002800
.set IC1_VADDR, 0x7e002830

/* main entry point */

.section .text.start
.globl _start
.align 2
_start:
	version r0
	mov r5, r0
        b prepare_stack

.altmacro
.macro RegExceptionHandler label, exception_number
  .globl vector_\exception_number
  vector_\exception_number :
  .long fleh_\label
.endm
.section .rodata.vectorTable
.globl vectorTable
vectorTable:
	RegExceptionHandler zero, 0
	RegExceptionHandler misaligned, 1
	RegExceptionHandler dividebyzero, 2
	RegExceptionHandler undefinedinstruction, 3
	RegExceptionHandler forbiddeninstruction, 4
	RegExceptionHandler illegalmemory, 5
	RegExceptionHandler buserror, 6
	RegExceptionHandler floatingpoint, 7
	RegExceptionHandler isp, 8
	RegExceptionHandler dummy, 9
	RegExceptionHandler icache, 10
	RegExceptionHandler veccore, 11
	RegExceptionHandler badl2alias, 12
	RegExceptionHandler breakpoint, 13
// vector 14 to 31 are all fleh_unknown
.set i, 14
.rept 18
  RegExceptionHandler unknown, %i
  .set i, i+1
.endr
// vector 32 to 143 are all fleh_irq
.set i,32
.rept 112
  RegExceptionHandler irq, %i
  .set i, i+1
.endr
.text
prepare_stack:
	/*
	 * load the interrupt and normal stack pointers. these
	 * are chosen to be near the top of the available cache memory
	 */

        // https://github.com/hermanhermitage/videocoreiv/wiki/VideoCore-IV-Programmers-Manual
        // r28 will become sp within an interrupt context
	mov r28, #0x1D000
	mov sp, #0x1C000

	/* jump to C code */
	mov r0, r5
	lea r1, _start

	bl _main

/************************************************************
 * Exception Handling
 ************************************************************/

.macro SaveRegsLower
	stm lr, (--sp)
	stm r0-r5, (--sp)
.endm

.macro SaveRegsUpper
	stm r6-r15, (--sp)
	stm r16-r23, (--sp)
.endm

.macro SaveRegsAll
	SaveRegsLower
	SaveRegsUpper
.endm

fatal_exception:
	SaveRegsUpper
	mov r0, sp
	b sleh_fatal

.macro ExceptionHandler label, exception_number
fleh_\label:
	SaveRegsLower
	mov r1, \exception_number
	b fatal_exception
.endm

	ExceptionHandler zero, #0
	ExceptionHandler misaligned, #1
	ExceptionHandler dividebyzero, #2
	ExceptionHandler undefinedinstruction, #3
	ExceptionHandler forbiddeninstruction, #4
	ExceptionHandler illegalmemory, #5
	ExceptionHandler buserror, #6
	ExceptionHandler floatingpoint, #7
	ExceptionHandler isp, #8
	ExceptionHandler dummy, #9
	ExceptionHandler icache, #10
	ExceptionHandler veccore, #11
	ExceptionHandler badl2alias, #12
	ExceptionHandler breakpoint, #13
	ExceptionHandler unknown, #14

fleh_irq:
	SaveRegsAll

	/* top of savearea */
	mov r0, sp
	mov r1, r29
	bl sleh_irq

return_from_exception:
	ldm r16-r23, (sp++)
	ldm r6-r15, (sp++)
	ldm r0-r5, (sp++)
	ld lr, (sp++)
	rti
