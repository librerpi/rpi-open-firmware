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
VideoCoreIV first stage bootloader.

=============================================================================*/

#include <runtime.h>
#include <hardware.h>
#include "romstage.h"
#include "sdram.h"
#include "arm_monitor.h"
#include "BCM2708PlatformStartup.h"
#include "utils.hh"
//#include "xprintf.h"
#include <stdio.h>
#include "pl011.h"
#include "hang_cpu.h"
#include "pll_read.h"
#include "interrupt.h"
#include "traps.h"

uint32_t g_CPUID;

static void switch_vpu_to_pllc() {
	A2W_XOSC_CTRL |= A2W_PASSWORD | A2W_XOSC_CTRL_PLLCEN_SET;

	A2W_PLLC_FRAC = A2W_PASSWORD | 87380;
	A2W_PLLC_CTRL = A2W_PASSWORD | 52 | 0x1000;

	A2W_PLLC_ANA3 = A2W_PASSWORD | 0x100;
	A2W_PLLC_ANA2 = A2W_PASSWORD | 0x0;
	A2W_PLLC_ANA1 = A2W_PASSWORD | 0x144000;
	A2W_PLLC_ANA0 = A2W_PASSWORD | 0x0;

	CM_PLLC = CM_PASSWORD | CM_PLLC_DIGRST_SET;

	/* hold all */
	CM_PLLC = CM_PASSWORD | CM_PLLC_DIGRST_SET |
	          CM_PLLC_HOLDPER_SET | CM_PLLC_HOLDCORE2_SET |
	          CM_PLLC_HOLDCORE1_SET | CM_PLLC_HOLDCORE0_SET;

	A2W_PLLC_DIG3 = A2W_PASSWORD | 0x0;
	A2W_PLLC_DIG2 = A2W_PASSWORD | 0x400000;
	A2W_PLLC_DIG1 = A2W_PASSWORD | 0x5;
	A2W_PLLC_DIG0 = A2W_PASSWORD | 52 | 0x555000;

	A2W_PLLC_CTRL = A2W_PASSWORD | 52 | 0x1000 | A2W_PLLC_CTRL_PRSTN_SET;

	A2W_PLLC_DIG3 = A2W_PASSWORD | 0x42;
	A2W_PLLC_DIG2 = A2W_PASSWORD | 0x500401;
	A2W_PLLC_DIG1 = A2W_PASSWORD | 0x4005;
	A2W_PLLC_DIG0 = A2W_PASSWORD | 52 | 0x555000;

	A2W_PLLC_CORE0 = A2W_PASSWORD | 2;

	CM_PLLC = CM_PASSWORD | CM_PLLC_DIGRST_SET |
	          CM_PLLC_HOLDPER_SET | CM_PLLC_HOLDCORE2_SET |
	          CM_PLLC_HOLDCORE1_SET | CM_PLLC_HOLDCORE0_SET | CM_PLLC_LOADCORE0_SET;

	CM_PLLC = CM_PASSWORD | CM_PLLC_DIGRST_SET |
	          CM_PLLC_HOLDPER_SET | CM_PLLC_HOLDCORE2_SET |
	          CM_PLLC_HOLDCORE1_SET | CM_PLLC_HOLDCORE0_SET;

	CM_PLLC = CM_PASSWORD | CM_PLLC_DIGRST_SET |
	          CM_PLLC_HOLDCORE2_SET |
	          CM_PLLC_HOLDCORE1_SET;

	CM_VPUCTL = CM_PASSWORD | CM_VPUCTL_FRAC_SET | CM_SRC_OSC | CM_VPUCTL_GATE_SET;
	CM_VPUDIV = CM_PASSWORD | (4 << 12);
	CM_VPUCTL = CM_PASSWORD | CM_SRC_PLLC_CORE0 | CM_VPUCTL_GATE_SET;
	CM_VPUCTL = CM_PASSWORD | CM_SRC_PLLC_CORE0 | CM_VPUCTL_GATE_SET | 0x10; /* ENAB */

	CM_TIMERDIV = CM_PASSWORD | (19 << 12) | 819;
	CM_TIMERCTL = CM_PASSWORD | CM_SRC_OSC | 0x10;
}

int _main(unsigned int cpuid, uint32_t load_address, vc4_saved_state_t* pcb) {
  xtal_freq = 19200000;
  switch_vpu_to_pllc();

  set_pl011_funcs();
  pl011_uart_init(115200);

  printf("CM_UARTCTL is 0x%08lx\nCM_UARTDIV is 0x%08lx\n", CM_UARTCTL, CM_UARTDIV);
  if (pcb) print_vpu_state(pcb);

  setup_irq_handlers();

	printf(
	    "Booting Raspberry Pi....\n"
	    "Copyright 2016-2017 rpi-open-firmware authors \n"
	    "BUILDATE  : %s %s \n",
	    __DATE__, __TIME__
	);

	g_CPUID = cpuid;

	__cxx_init();

	/* bring up SDRAM */
	sdram_init();
	puts("SDRAM initialization completed successfully!\n");

        setup_eth_clock(4);

	PEStartPlatform();
  __asm__ volatile("ei");

	/* start vpu monitor */
	monitor_start();

	panic("main exiting!");
}
