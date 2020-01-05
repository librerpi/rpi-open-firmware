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
Glue code for Broadcom's register definitions as well as certain registers
that are missing from the release. This is also used by ARM.

=============================================================================*/

#include <stdint.h>

#pragma once

#define VC4_PERIPH_BASE 0x7E000000
#define ARM_PERIPH_BASE 0x3F000000

#define VC4_TO_ARM_PERIPH(addr) ((addr - VC4_PERIPH_BASE) + ARM_PERIPH_BASE)

#define VC4_CPUID_BCM2709_PLUS 0x40

#ifdef __arm__
#define HW_REGISTER_RW(addr) (*(volatile uint32_t *)(VC4_TO_ARM_PERIPH(addr)))
#define HW_REGISTER_RO(addr) (*(const volatile uint32_t *)(VC4_TO_ARM_PERIPH(addr)))
#else
#define HW_REGISTER_RW(addr) (*(volatile uint32_t *)(addr))
#define HW_REGISTER_RO(addr) (*(const volatile uint32_t *)(addr))
#endif

#define mmio_read32(addr) HW_REGISTER_RW(addr)
#define mmio_write32(addr, value) (HW_REGISTER_RW(addr) = value)

#include "broadcom/hardware_vc4.h"

/*
 * this is not included by hardware_vc4.h
 */
#include "broadcom/bcm2708_chip/aux_io.h"
#include "broadcom/bcm2708_chip/testbus.h"

#define RAM_SIZE_1GB 0
#define RAM_SIZE_512MB 1
#define RAM_SIZE_256MB 2
#define RAM_SIZE_128MB 3
#define RAM_SIZE_UNKNOWN 4

/*
 * LPDDR mode registers.
 */
#define LPDDR2_MR_DEVICE_INFO      0
#define LPDDR2_MR_DEVICE_FEATURE_1 1
#define LPDDR2_MR_DEVICE_FEATURE_2 2
#define LPDDR2_MR_IO_CONFIG        3
#define LPDDR2_MR_TEMPERATURE      4
#define LPDDR2_MR_MANUFACTURER_ID  5
#define LPDDR2_MR_REV_1            6
#define LPDDR2_MR_REV_2            7
#define LPDDR2_MR_METRICS          8
#define LPDDR2_MR_CALIBRATION      10

#define CM_SRC_GND			0
#define CM_SRC_OSC			1
#define CM_SRC_TESTDEBUG0		2
#define CM_SRC_TESTDEBUG1		3
#define CM_SRC_PLLA_CORE		4
#define CM_SRC_PLLA_PER		4
#define CM_SRC_PLLC_CORE0		5
#define CM_SRC_PLLC_PER		5
#define CM_SRC_PLLC_CORE1		8
#define CM_SRC_PLLD_CORE		6
#define CM_SRC_PLLD_PER		6
#define CM_SRC_PLLH_AUX		7
#define CM_SRC_PLLC_CORE1		8
#define CM_SRC_PLLC_CORE2		9

typedef struct {
	uint32_t sdram_size;
	uint32_t vpu_cpuid;
	uint32_t reserved[3];
} firmware_arm_data_t;

#ifdef __arm__
extern firmware_arm_data_t g_FirmwareData;
#endif
