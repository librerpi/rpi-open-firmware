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

#include <lib/runtime.h>
#include <hardware.h>
#include "romstage.h"
#include "sdram.h"
#include "arm_monitor.h"
#include "BCM2708PlatformStartup.h"

uint32_t g_CPUID;

#define UART_IBRD   (UART_BASE+0x24)
#define UART_FBRD   (UART_BASE+0x28)
#define UART_LCRH   (UART_BASE+0x2C)
#define UART_CR     (UART_BASE+0x30)
#define UART_ICR    (UART_BASE+0x44)

void uart_putc(unsigned int ch) {
	while(UART_MSR & 0x20);
	UART_RBRTHRDLL = ch;
}

void uart_init(void) {
	unsigned int ra = GP_FSEL1;
	ra &= ~(7 << 12);
	ra |= 4 << 12;
	ra &= ~(7 << 15);
	ra |= 4 << 15;
	GP_FSEL1 = ra;

	mmio_write32(UART_CR, 0);

	GP_PUD = 0;
	udelay(150);
	GP_PUDCLK0 = (1 << 14) | (1 << 15);
	udelay(150);
	GP_PUDCLK0 = 0;

	CM_UARTDIV = CM_PASSWORD | 0x6666;
	CM_UARTCTL = CM_PASSWORD | CM_SRC_OSC | CM_UARTCTL_FRAC_SET | CM_UARTCTL_ENAB_SET;

	mmio_write32(UART_ICR, 0x7FF);
	mmio_write32(UART_IBRD, 1);
	mmio_write32(UART_FBRD, 40);
	mmio_write32(UART_LCRH, 0x70);
	mmio_write32(UART_CR, 0x301);
}

void switch_vpu_to_pllc() {
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
	          CM_PLLC_HOLDPER_SET | CM_PLLC_HOLDCORE2_SET |
	          CM_PLLC_HOLDCORE1_SET;

	CM_VPUCTL = CM_PASSWORD | CM_VPUCTL_FRAC_SET | CM_SRC_OSC | CM_VPUCTL_GATE_SET;
	CM_VPUDIV = CM_PASSWORD | (4 << 12);
	CM_VPUCTL = CM_PASSWORD | CM_SRC_PLLC_CORE0 | CM_VPUCTL_GATE_SET;
	CM_VPUCTL = CM_PASSWORD | CM_SRC_PLLC_CORE0 | CM_VPUCTL_GATE_SET | 0x10; /* ENAB */

	CM_TIMERDIV = CM_PASSWORD | (19 << 12) | 819;
	CM_TIMERCTL = CM_PASSWORD | CM_SRC_OSC | 0x10;
}

void set_interrupt(int intno, bool enable, int core) {
    int base = (core == 0) ? IC0_BASE : IC1_BASE;

    int offset = 0x10 + ((intno >> 3) << 2);
    uint32_t slot = 0xF << ((intno & 7) << 2);

    uint32_t v = mmio_read32(base + offset) & ~slot;
    mmio_write32(base + offset, enable ? v | slot : v);
}

int _main(unsigned int cpuid, unsigned int load_address) {
	switch_vpu_to_pllc();

	uart_init();

	for(int i = 0; i < 64; ++i) {
	    set_interrupt(i, (i != (125 - 64)) && (i != (121 - 64)) && (i != (120 - 64)) && (i != (73 - 64)) && (i != (96 - 64)), 0);
	    set_interrupt(i, 0, 1);
	}

	IC0_VADDR = 0x1B000;
	IC1_VADDR = 0x1B000;

	__asm__ volatile("ei");

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
	printf("SDRAM initialization completed successfully!\n");

	PEStartPlatform();

	/* start vpu monitor */
	monitor_start();

	panic("main exiting!");
}
