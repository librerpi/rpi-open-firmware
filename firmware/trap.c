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
VideoCoreIV second level exception handlers.

=============================================================================*/

#include <runtime.h>
#include <pcb.h>
#include <exception.h>
#include <hardware.h>
#include <cpu.h>
#include <hang_cpu.h>
#include <stdio.h>
#include "arm_monitor.h"
#include "traps.h"

static const char* g_ExceptionNames[] = { VC4_EXC_NAMES };

static const char* exception_name(uint32_t n) {
	if (n >= (sizeof(g_ExceptionNames)/4))
		return "unknown";
	return g_ExceptionNames[n];
}

#define REGISTER_FORMAT_STRING(prefix) \
	prefix "  r0: 0x%08lx  r1: 0x%08lx  r2: 0x%08lx  r3: 0x%08lx\n" \
	prefix "  r4: 0x%08lx  r5: 0x%08lx  r6: 0x%08lx  r7: 0x%08lx\n" \
	prefix "  r8: 0x%08lx  r9: 0x%08lx r10: 0x%08lx r11: 0x%08lx\n" \
	prefix " r12: 0x%08lx r13: 0x%08lx r14: 0x%08lx r15: 0x%08lx\n" \
	prefix "  pc: 0x%08lx  lr: 0x%08lx  sr: 0x%08lx\n"

void print_vpu_state(vc4_saved_state_t* pcb) {
	printf("VPU registers:\n");

	printf(
	    REGISTER_FORMAT_STRING("   "),
	    pcb->r0,
	    pcb->r1,
	    pcb->r2,
	    pcb->r3,
	    pcb->r4,
	    pcb->r5,
	    pcb->r6,
	    pcb->r7,
	    pcb->r8,
	    pcb->r9,
	    pcb->r10,
	    pcb->r11,
	    pcb->r12,
	    pcb->r13,
	    pcb->r14,
	    pcb->r15,
	    pcb->pc,
	    pcb->lr,
	    pcb->sr
	);

	printf("Exception info (IC0):\n");

	printf(
	    "   src0: 0x%08lx src1: 0x%08lx vaddr: 0x%08lx\n"
	    "      C: 0x%08lx    S: 0x%08lx\n",
	    IC0_SRC0,
	    IC0_SRC1,
	    IC0_VADDR,
	    IC0_C,
	    IC0_S
	);

	printf("Exception info (IC1):\n");

	printf(
	    "   src0: 0x%08lx src1: 0x%08lx vaddr: 0x%08lx\n"
	    "      C: 0x%08lx    S: 0x%08lx\n",
	    IC1_SRC0,
	    IC1_SRC1,
	    IC1_VADDR,
	    IC1_C,
	    IC1_S
	);
        /*puts("stack dump:\n");
        uint8_t *ram = 0;
        uint32_t *ram32 = 0;
        uint32_t starting_point = 0x1C000 - 0x200;
        for (uint32_t i = starting_point & ~0xf; i < (starting_point + 0x200); i = i + 4) {
          printf("0x%x %08x\n", i, ram32[i/4]);
        }*/
}

// return_from_exception will read from pcb when restoring state, so that could be used to mutate the state
void sleh_fatal(vc4_saved_state_t* pcb, uint32_t n) {
	printf("Fatal VPU Exception: %s\n", exception_name(n));

	print_vpu_state(pcb);

	printf("We are hanging here ...\n");

	hang_cpu();
}

// when called, pcb contains a pointer to the entire register state, as made by SaveRegsAll
// return_from_exception will read from pcb when restoring state, so that could be used to mutate the state
// tp contains r29, the thread pointer
void sleh_irq(vc4_saved_state_t* pcb, uint32_t tp) {
  uint32_t status = IC0_S;
  uint32_t source = status & 0xFF;
  uint32_t cs;

  print_timestamp();

  printf("VPU Received interrupt from source %ld\n", source);

  switch (source) {
  case INTERRUPT_TIMER0:
    cs = ST_CS;
    ST_CS = cs;
    ST_C0 += 1 * 1000 * 1000;
    return;
    print_vpu_state(pcb);
    puts("unexpected timer0, did you forget about the pi while writing code?");
    break;
  case INTERRUPT_ARM:
    arm_monitor_interrupt();
    break;
  default:
    print_vpu_state(pcb);
    panic("unknown interrupt source!");
  }
}
