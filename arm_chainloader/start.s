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
ARM entry point.

This is where all cores start. For RPi1, only one core starts so we can jump
straight to the main bootloader. For later models, the first core jumps to the
bootloader. The other cores wait until SMP is enabled by the kernel later in
the boot process.

=============================================================================*/

#include "memory_map.h"
#include "arm-defs.h"

.arch_extension sec

.text
.globl _start
_start:
  /* vectors */
  b _common_start /* reset */
  b _fleh_undef /* undefined */
  b _secure_monitor /* swi/smc */
  b _fleh_prefabt /* prefetch abort */
  b _fleh_dataabt /* data abort */
  b _fleh_addrexc /* reserved */
  b _fleh_irq /* irq */
  b _fleh_fiq /* fiq */

.org 32
.globl g_FirmwareData
g_FirmwareData:
  .long 0 /* SDRAM capacity */
  .long 0 /* VPU CPUID      */
  .long 0 /* serial#        */
  .long 0 /* revision       */
  .long 0 /* Reserved */

# based on https://github.com/mrvn/moose/blob/master/kernel/entry.S
.macro SaveRegisters, offset
  # note, this will trash the stack of the main thread
  # it will also trash the stack of all other modes
  # therefore, any interupt at all is currently fatal
  mov sp, #(MEM_STACK_END)
  sub lr, #\offset
  stmea sp, {r0-lr}^
  str lr, [sp, #60]
  mrs r0, spsr
  str r0, [sp, #64]
  mov r0, sp
.endm

# based on https://github.com/mrvn/moose/blob/master/kernel/entry.S
.macro NewSaveRegisters, offset
  sub lr, #\offset
  stmea sp, {r0-lr}^
  str lr, [sp, #60]
  mrs r0, spsr
  str r0, [sp, #64]
  mov r0, sp
.endm

_fleh_undef:
  SaveRegisters 4
  b sleh_undef

_fleh_prefabt:
  SaveRegisters 4
  b sleh_prefabt

_fleh_dataabt:
  SaveRegisters 8
  b sleh_dataabt

_fleh_addrexc:
  SaveRegisters 0
  b sleh_addrexc

_fleh_irq:
  // TODO, dont bother saving the FIQ swapped regs
  SaveRegisters 4
  b sleh_irq

_fleh_fiq:
  NewSaveRegisters 4
  b sleh_fiq

_secure_monitor:
  mrc p15, 0, r0, c1, c1, 0 // SCR -> r0
  //bic	r0, r0, #0x4a       /* clear IRQ, EA, nET */
  orr r0, r0, #1            /* set NS */
  mcr p15, 0, r0, c1, c1, 0 // r0 -> SCR, now with NS set

  //mov r0, #((1 << 7) | (1 << 8) | (1 << 6)) /* mask IRQ, AA and FIQ */
  //orr r0, r0, #0x1a       /* switch to hypervisor mode */
  //msr spsr_cxfs, r0

  movs pc, lr

.macro SetModeSp, mode, newsp
  mov r1, r0
  bic r1, r1, #(ARM32_MODE_MASK) // clear all mode bits
  orr r1, r1, #(\mode)      // set a mode
  msr cpsr, r1              // enter the mode
  ldr sp, =\newsp
.endm

.global asm_enable_fpu
asm_enable_fpu:
  mrc p15, 0, r0, c1, c1, 2 // read NSACR -> r0
  orr r0, r0, #3<<10        // enable FPU
  mcr p15, 0, r0, c1, c1, 2 // write back to NSACR
  ldr r0, =(0xF << 20)
  mcr p15, 0, r0, c1, c0, 2 // CPACR
  mov r0, #0x40000000
  vmsr fpexc, r0
  bx lr
.global asm_drop_secure
asm_drop_secure:
  // set non-secure flag
  mrc p15, 0, r0, c1, c1, 0 // SCR -> r0
  orr r0, r0, #1 /* set NS */
  mcr p15, 0, r0, c1, c1, 0 // r0 -> SCR, but with NS set
  bx lr
.global asm_set_ACTLR
asm_set_ACTLR:
  // based on https://github.com/raspberrypi/tools/blob/509f504e8f130ead31b85603016d94d5c854c10c/armstubs/armstub7.S#L78-L82
  mrc p15, 0, r1, c1, c0, 1 @ Read Auxiliary Control Register
  orr r1, r1, r0
  mcr p15, 0, r1, c1, c0, 1 @ Write Auxiliary Control Register
  bx lr

.global set_CPUECTLR
set_CPUECTLR:
  // based on https://github.com/raspberrypi/tools/blob/509f504e8f130ead31b85603016d94d5c854c10c/armstubs/armstub7.S#L83-L86
  mrrc p15, 1, r0, r1, c15  @ CPU Extended Control Register
  orr r0, r0, #(1<<6)       @ SMP
  and r1, r1, #(~3)         @ Set L2 load data prefetch to 0b00 = 16
  mcrr p15, 1, r0, r1, c15  @ CPU Extended Control Register
  bx lr

.global enable_icache
enable_icache:
  mrc p15, 0, r0, c1, c0, 0 // SCTLR -> r0
  orr r0, r0, #(1<<12)      // r0 |= 1<<12 icache
  mcr p15, 0, r0, c1, c0, 0 // r0 -> SCTLR
  bx lr

.global disable_icache
disable_icache:
  mrc p15, 0, r0, c1, c0, 0 // SCTLR -> r0
  bic r0, r0, #(1<<12)      // clear bit 12 icache
  mcr p15, 0, r0, c1, c0, 0 // r0 -> SCTLR
  bx lr

.global lock_obtain
lock_obtain:
  mov r1, #0xff           // lock taken flag
try:
  ldrex r2, [r0]          // load lock value
  cmp r2, #0              // check if lock free
  strexeq r1, r2, [r0]    // if free, try to write as taken
  cmpeq r2, #0            // if was free, check if write worked
  bne try                 // if write failed, retry
  bx lr

_common_start:
  mrs r0, cpsr

  SetModeSp ARM32_IRQ, _irq_stack_top
  SetModeSp ARM32_SVC, _svc_stack_top

  msr cpsr, r0 // revert to previous mode
  /*
   * read MIDR, see if this is an ARMv6 system, if it is, just
   * assume single core (BCM2708) and not bother doing SMP stuff.
   */
  mrc p15, 0, r0, c0, c0, 0 // MIDR -> r0
  lsr r0, #16
  and r0, #0xF
  cmp r0, #0x7              // check if MIDR ends in 0x7__
  beq L_finish_init

L_armv7_or_higher:
  /*
   * okay, we're an ARMv7 or an ARMv8.
   */

  mrc p15, 0, r0, c0, c0, 5   // read MPIDR
  and r3, r0, #0xc0000000     // multiprocessing extensions and
  teq r3, #0x80000000         // not part of a uniprocessor system?
  bne L_setup_monitor         // no, assume UP

  mrc p15, 0, r0, c0, c0, 5   // read MPIDR
  ands r0, r0, #0x03          // CPU 0?
  //cmp r0, #2
  //bne L_deadloop            // if not, spin.

  cmp r0, #3
  ldrls r0, [pc, r0, lsl #2]
  b after_table
  .word 0x24000
  .word 0x28000
  .word 0x2c000
  .word 0x30000
after_table:
  mov sp, r0

L_setup_monitor:
  //adr	r1, _start
  //mcr	p15, 0, r1, c12, c0, 1 /* MVBAR */
  //mcr p15, 0, r1, c7, c5, 4 /* ISB (ARMv6 compatible way) */


  //smc 0

L_finish_init:
  /* enable instruction cache */
  //mrc p15, 0, r0, c1, c0, 0   // SCTLR -> r0
  //orr r0, r0, #(1<<12)        // r0 |= 1<<12 i-cache enable
  //mcr p15, 0, r0, c1, c0, 0   // r0 -> SCTLR

bl c_entry
b .

L_deadloop:
  mov r0, #0x43
  //bl uart_putc
  // TODO, break out of loop when signaled by linux
  cpsie if
  wfi
  b L_deadloop
L_wordtable:
