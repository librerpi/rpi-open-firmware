/*=============================================================================
Copyright (C) 2016-2017 Authors of rpi-open-firmware
Copyright (C) 2016 Julian Brown
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
VideoCoreIV SDRAM initialization code.

=============================================================================*/

#include <runtime.h>
#include <hardware.h>
#include "romstage.h"
#include "sdram.h"

/*
 Registers
 =========

 SC: AC Timing (Page 202)
 SB: ???
 SD: AC Timing (Page 202)
 SE: AC Timing (Page 202)

 PT1:
	Minimum Idle time after first CKE assertion
	Minimum CKE low time after completion of power ramp
 PT2:
	DAI Duration
 */

#define MR_REQUEST_SUCCESS(x) ((SD_MR_TIMEOUT_SET & x) != SD_MR_TIMEOUT_SET)
#define MR_GET_RDATA(x) ((x & SD_MR_RDATA_SET) >> SD_MR_RDATA_LSB)

#define SIP_DEBUG(x) x
#define SCLKU_DEBUG(x) //SIP_DEBUG(x)

#define BIST_pvt    0x20
#define BIST_reset  0x10

#define PVT_calibrate_request 0x1

#define logf(fmt, ...) printf("[SDRAM:%s]: " fmt, __FUNCTION__, ##__VA_ARGS__);

uint32_t g_RAMSize = RAM_SIZE_UNKNOWN;

static const char* lpddr2_manufacturer_name(uint32_t mr) {
	switch (mr) {
	case 1:
		return "Samsung";
	case 2:
		return "Qimonda";
	case 3:
		return "Elpida";
	case 4:
		return "Etron";
	case 5:
		return "Nanya";
	case 6:
		return "Hynix";
	default:
		return "Unknown";
	}
}

#define MR8_DENSITY_SHIFT	0x2
#define MR8_DENSITY_MASK	(0xF << 0x2)

static unsigned lpddr2_size(uint32_t mr) {
	switch (mr) {
	case 0x58:
		return RAM_SIZE_1GB;
	case 0x18:
		return RAM_SIZE_512MB;
	case 0x14:
		return RAM_SIZE_256MB;
	case 0x10:
		return RAM_SIZE_128MB;
	default:
		return RAM_SIZE_UNKNOWN;
	}
}

const char* size_to_string[] = {
	"1GB",
	"512MB",
	"256MB",
	"128MB",
	"UNKNOWN"
};

/*****************************************************************************
 * Guts
 *****************************************************************************/

ALWAYS_INLINE inline void clkman_update_begin() {
	CM_SDCCTL |= CM_PASSWORD | CM_SDCCTL_UPDATE_SET;
	SCLKU_DEBUG(logf("waiting for ACCPT (%X) ...\n", CM_SDCCTL));
	for (;;) if (CM_SDCCTL & CM_SDCCTL_ACCPT_SET) break;
	SCLKU_DEBUG(logf("ACCPT received! (%X)\n", CM_SDCCTL));
}

ALWAYS_INLINE inline void clkman_update_end() {
	CM_SDCCTL = CM_PASSWORD | (CM_SDCCTL & CM_SDCCTL_UPDATE_CLR);
	SCLKU_DEBUG(logf("waiting for ACCPT clear (%X) ...\n", CM_SDCCTL));
	for (;;) if ((CM_SDCCTL & CM_SDCCTL_ACCPT_SET) == 0) break;
	SCLKU_DEBUG(logf("ACCPT cleared! (%X)\n", CM_SDCCTL));
}

ALWAYS_INLINE void reset_phy_dll() {
	SIP_DEBUG(logf("resetting aphy and dphy dlls ...\n"));

	/* politely tell sdc that we'll be messing with address lines */
	APHY_CSR_PHY_BIST_CNTRL_SPR = 0x30;

	DPHY_CSR_GLBL_DQ_DLL_RESET = 0x1;
	APHY_CSR_GLBL_ADDR_DLL_RESET = 0x1;

	/* stall ... */
	SD_CS;
	SD_CS;
	SD_CS;
	SD_CS;

	DPHY_CSR_GLBL_DQ_DLL_RESET = 0x0;
	APHY_CSR_GLBL_ADDR_DLL_RESET = 0x0;

	SIP_DEBUG(logf("waiting for dphy master dll to lock ...\n"));
	for (;;) if ((DPHY_CSR_GLBL_MSTR_DLL_LOCK_STAT & 0xFFFF) == 0xFFFF) break;
	SIP_DEBUG(logf("dphy master dll locked!\n"));
}

typedef struct {
	uint32_t max_freq;
	uint32_t RL;
	uint32_t tRPab;
	uint32_t tRPpb;
	uint32_t tRCD;
	uint32_t tWR;
	uint32_t tRASmin;
	uint32_t tRRD;
	uint32_t tWTR;
	uint32_t tXSR;
	uint32_t tXP;
	uint32_t tRFCab;
	uint32_t tRTP;
	uint32_t tCKE;
	uint32_t tCKESR;
	uint32_t tDQSCKMAXx2;
	uint32_t tRASmax;
	uint32_t tFAW;
	uint32_t tRC;
	uint32_t tREFI;

	uint32_t tINIT1;
	uint32_t tINIT3;
	uint32_t tINIT5;

	uint32_t rowbits;
	uint32_t colbits;
	uint32_t banklow;
} lpddr2_timings_t;

// 7.8 / (1.0 / 400)

lpddr2_timings_t g_InitSdramParameters = {
	/* SA (us) */
	.tREFI = 3113, //Refresh rate: 3113 * (1.0 / 400) = 7.78us
	/* SC (ns) */
	.tRFCab = 50,
	.tRRD = 2,
	.tWR = 7,
	.tWTR = 4,
	/* SD (ns) */
	.tRPab = 7,
	.tRC = 24,
	.tXP = 1,
	.tRASmin = 15,
	.tRPpb = 6,
	.tRCD = 6,
	/* SE (ns) */
	.tFAW = 18,
	.tRTP = 1,
	.tXSR = 54,
	/* PT */
	.tINIT1 = 40, // Minimum CKE low time after completion of power ramp: 40 * (1.0 / 0.4) = 100ns
	.tINIT3 = 79800, // Minimum Idle time after first CKE assertion: 79800 * (1.0 / 400) = 199.5us ~ 200us
	.tINIT5 = 3990, //Max DAI: 3990* (1.0 / 400) = 9.9us ~ 10us
	/* SB */
	.rowbits = 2,
	.colbits = 1,
	.banklow = 2
};

void reset_with_timing(lpddr2_timings_t* T) {
	uint32_t ctrl = 0x4;

	SD_CS = (SD_CS & ~(SD_CS_DEL_KEEP_SET|SD_CS_DPD_SET|SD_CS_RESTRT_SET)) | SD_CS_STBY_SET;

	/* wait for SDRAM controller to go down */
	SIP_DEBUG(logf("waiting for SDRAM controller to go down (%lX) ...\n", SD_CS));
	for (;;) if ((SD_CS & SD_CS_SDUP_SET) == 0) break;
	SIP_DEBUG(logf("SDRAM controller down!\n"));

	/* disable SDRAM clock */
	clkman_update_begin();
	CM_SDCCTL = (CM_SDCCTL & ~(CM_SDCCTL_ENAB_SET|CM_SDCCTL_CTRL_SET)) | CM_PASSWORD;
	clkman_update_end();

	SIP_DEBUG(logf("SDRAM clock disabled!\n"));

	/*
	 * Migrate over to master PLL.
	 */

	APHY_CSR_DDR_PLL_PWRDWN = 0;
	APHY_CSR_DDR_PLL_GLOBAL_RESET = 0;
	APHY_CSR_DDR_PLL_POST_DIV_RESET = 0;

	/* 400MHz */
	APHY_CSR_DDR_PLL_VCO_FREQ_CNTRL0 = (1 << 16) | 0x53;
	APHY_CSR_DDR_PLL_VCO_FREQ_CNTRL1 = 0;
	APHY_CSR_DDR_PLL_MDIV_VALUE = 0;

	APHY_CSR_DDR_PLL_GLOBAL_RESET = 1;

	SIP_DEBUG(logf("waiting for master ddr pll to lock ...\n"));
	for (;;) if (APHY_CSR_DDR_PLL_LOCK_STATUS & (1 << 16)) break;
	SIP_DEBUG(logf("master ddr pll locked!\n"));

	APHY_CSR_DDR_PLL_POST_DIV_RESET = 1;

	clkman_update_begin();
	CM_SDCCTL = CM_PASSWORD | (ctrl << CM_SDCCTL_CTRL_LSB) | (CM_SDCCTL & CM_SDCCTL_CTRL_CLR);
	clkman_update_end();

	SD_SA =
	    (T->tREFI << SD_SA_RFSH_T_LSB)
	    | SD_SA_PGEHLDE_SET
	    | SD_SA_CLKSTOP_SET
	    | SD_SA_POWSAVE_SET
	    | 0x3214;

	SD_SB =
	    SD_SB_REORDER_SET
	    | (T->banklow << SD_SB_BANKLOW_LSB)
	    | SD_SB_EIGHTBANK_SET
	    | (T->rowbits << SD_SB_ROWBITS_LSB)
	    | (T->colbits << SD_SB_COLBITS_LSB);

	logf("SDRAM Addressing Mode: Bank=%ld Row=%ld Col=%ld SB=0x%lX\n", T->banklow, T->rowbits, T->colbits, SD_SB);

	SD_SC =
	    (T->tRFCab << SD_SC_T_RFC_LSB)
	    | (T->tRRD << SD_SC_T_RRD_LSB)
	    | (T->tWR << SD_SC_T_WR_LSB)
	    | (T->tWTR << SD_SC_T_WTR_LSB)
	    | (3 << SD_SC_WL_LSB);

	SD_SD =
	    (T->tRPab << SD_SD_T_RPab_LSB)
	    | (T->tRC << SD_SD_T_RC_LSB)
	    | (T->tXP << SD_SD_T_XP_LSB)
	    | (T->tRASmin << SD_SD_T_RAS_LSB)
	    | (T->tRPpb << SD_SD_T_RPpb_LSB)
	    | (T->tRCD << SD_SD_T_RCD_LSB);

	SD_SE =
	    (1 << SD_SE_RL_EN_LSB)
	    | (4 << SD_SE_RL_LSB)
	    | (T->tFAW << SD_SE_T_FAW_LSB)
	    | (T->tRTP << SD_SE_T_RTP_LSB)
	    | (T->tXSR << SD_SE_T_XSR_LSB);

	SD_PT1 =
	    (T->tINIT3 << SD_PT1_T_INIT3_LSB)
	    | (T->tINIT1 << SD_PT1_T_INIT1_LSB);

	SD_PT2 =
	    T->tINIT5 << SD_PT2_T_INIT5_LSB;

	SD_MRT =
	    0x3 << SD_MRT_T_MRW_LSB;

	reset_phy_dll();

	/* wait for address line pll to come back */
	SIP_DEBUG(logf("waiting for address dll to lock ...\n"));
	for (;;) if (APHY_CSR_GLBL_ADR_DLL_LOCK_STAT == 3) break;
	SIP_DEBUG(logf("address dll locked!\n"));

	/* tell sdc we're done messing with address lines */
	APHY_CSR_PHY_BIST_CNTRL_SPR = 0x0;

	/* woo, turn on sdram! */
	SD_CS =
	    (((4 << SD_CS_ASHDN_T_LSB)
	      | SD_CS_STATEN_SET
	      | SD_CS_EN_SET)
	     & ~(SD_CS_STOP_SET|SD_CS_STBY_SET)) | SD_CS_RESTRT_SET;
}

unsigned int read_mr(unsigned int addr) {
	while ((SD_MR & SD_MR_DONE_SET) != SD_MR_DONE_SET) {}
	SD_MR = addr & 0xFF;
	unsigned int mrr;
	while (((mrr = SD_MR) & SD_MR_DONE_SET) != SD_MR_DONE_SET) {}
	return mrr;
}

static unsigned int write_mr(unsigned int addr, unsigned int data, bool wait) {
	while ((SD_MR & SD_MR_DONE_SET) != SD_MR_DONE_SET) {}

	SD_MR = (addr & 0xFF) | ((data & 0xFF) << 8) | SD_MR_RW_SET;

	if (wait) {
		unsigned int mrr;
		while (((mrr = SD_MR) & SD_MR_DONE_SET) != SD_MR_DONE_SET) {}

		if (mrr & SD_MR_TIMEOUT_SET)
			panic("MR write timed out (addr=%d data=0x%X)", addr, data);

		return mrr;
	} else {
		return 0;
	}
}

void reset_phy() {
	logf("%s: resetting SDRAM PHY ...\n", __FUNCTION__);

	/* reset PHYC */
	SD_PHYC = SD_PHYC_PHYRST_SET;
	udelay(64);
	SD_PHYC = 0;

	logf("%s: resetting DPHY CTRL ...\n", __FUNCTION__);

	DPHY_CSR_DQ_PHY_MISC_CTRL = 0x7;
	DPHY_CSR_DQ_PAD_MISC_CTRL = 0x0;
	DPHY_CSR_BOOT_READ_DQS_GATE_CTRL = 0x11;

	reset_phy_dll();

	APHY_CSR_PHY_BIST_CNTRL_SPR = 0x0;
}

static void switch_to_cprman_clock(unsigned int source, unsigned int div) {
	CM_SDCDIV = CM_PASSWORD | (div << CM_SDCDIV_DIV_LSB);
	CM_SDCCTL = CM_PASSWORD | (CM_SDCCTL & CM_SDCCTL_SRC_CLR) | source;
	CM_SDCCTL |= CM_PASSWORD | CM_SDCCTL_ENAB_SET;

	logf("switching sdram to cprman clock (src=%d, div=%d), waiting for busy (%lX) ...\n", source, div, CM_SDCCTL);

	for (;;) if (CM_SDCCTL & CM_SDCCTL_BUSY_SET) break;

	logf("busy set, switch complete!\n");
}

static void init_clkman() {
	uint32_t ctrl = 0;

	clkman_update_begin();
	CM_SDCCTL = CM_PASSWORD | (ctrl << CM_SDCCTL_CTRL_LSB) | (CM_SDCCTL & CM_SDCCTL_CTRL_CLR);
	clkman_update_end();
}

#define CALL_INIT_CLKMAN init_clkman();


/*****************************************************************************
 * Calibration
 *****************************************************************************/

static void calibrate_pvt_early() {
	/* some hw revisions require different slews */
  // tests for a cpuid ending in 0x___14_
	bool st = ((g_CPUID >> 4) & 0xFFF) == 0x14;
	uint32_t dq_slew = (st ? 2 : 3);

	/* i don't get it, the spec says do not use this register */
	write_mr(0xFF, 0, true);
	/* RL = 6 / WL = 3 */
	write_mr(LPDDR2_MR_DEVICE_FEATURE_2, 4, true);

	APHY_CSR_ADDR_PAD_DRV_SLEW_CTRL = 0x333;
	DPHY_CSR_DQ_PAD_DRV_SLEW_CTRL = (dq_slew << 8) | (dq_slew << 4) | 3;

	logf("DPHY_CSR_DQ_PAD_DRV_SLEW_CTRL = 0x%lX\n", DPHY_CSR_DQ_PAD_DRV_SLEW_CTRL);

	/* tell sdc we want to calibrate */
	APHY_CSR_PHY_BIST_CNTRL_SPR = BIST_pvt;

	/* pvt compensation */
	APHY_CSR_ADDR_PVT_COMP_CTRL = PVT_calibrate_request;
	logf("waiting for address PVT calibration ...\n");
	for (;;) if (APHY_CSR_ADDR_PVT_COMP_STATUS & 2) break;

	DPHY_CSR_DQ_PVT_COMP_CTRL = PVT_calibrate_request;
	logf("waiting for data PVT calibration ...\n");
	for (;;) if (DPHY_CSR_DQ_PVT_COMP_STATUS & 2) break;

	/* tell sdc we're done calibrating */
	APHY_CSR_PHY_BIST_CNTRL_SPR = 0x0;

	/* send calibration command */
	uint32_t old_mrt = SD_MRT;
	SD_MRT = 20;
	logf("waiting for SDRAM calibration command ...\n");
	SD_MR = LPDDR2_MR_CALIBRATION | (0xFF << 8) | SD_MR_RW_SET | SD_MR_HI_Z_SET;
	while ((SD_MR & SD_MR_DONE_SET) != SD_MR_DONE_SET) {}
	SD_MRT = old_mrt;

	write_mr(LPDDR2_MR_IO_CONFIG, st ? 3 : 2, false);
}


/*****************************************************************************
 * Late init
 *****************************************************************************/

static void init_late() {
}

/*****************************************************************************
 * Self-test
 *****************************************************************************/

#define RT_BASE 0xC0000000

#define RT_PAT0 0xAAAAAAAA
#define RT_PAT1 0xFF00AA00
#define RT_PAT2 0x99999999

#define RT_ASSERT(i_, expected) \
	if (ram[(i_)] != expected) { \
		logf("ERROR: At 0x%p, was expecting 0x%X from read, got 0x%lX instead!\n", \
			&ram[(i_)], \
			expected, \
			ram[(i_)]); \
		panic("SDRAM self test failed!"); \
	}

static void selftest_at(uint32_t addr) {
	volatile uint32_t* ram = (volatile uint32_t*)addr;

	logf("Testing region at 0x%lX ...\n", addr);

	for (int i = 0; i < 0x1000; i += 4) {
		ram[i]     = RT_PAT0;
		ram[i + 1] = RT_PAT1;
		ram[i + 2] = RT_PAT2;
		ram[i + 3] = RT_PAT0;
	}

	for (int i = 0; i < 0x1000; i += 4) {
		RT_ASSERT(i,     RT_PAT0);
		RT_ASSERT(i + 1, RT_PAT1);
		RT_ASSERT(i + 2, RT_PAT2);
		RT_ASSERT(i + 3, RT_PAT0);
	}
}

static void selftest() {
	logf("Starting self test ...\n");

	selftest_at(RT_BASE);

	if (g_RAMSize == RAM_SIZE_256MB || g_RAMSize == RAM_SIZE_512MB || g_RAMSize == RAM_SIZE_1GB) {
		selftest_at(RT_BASE + 0xFF00000);
	}
	if (g_RAMSize == RAM_SIZE_512MB || g_RAMSize == RAM_SIZE_1GB) {
		selftest_at(RT_BASE + 0x1FF00000);
	}
	if (g_RAMSize == RAM_SIZE_1GB) {
		selftest_at(RT_BASE + 0x2FF00000);
		selftest_at(RT_BASE + 0x3FF00000);
	}

	logf("Self test successful!\n");
}

#undef RT_ASSERT

void sdram_init() {
	uint32_t vendor_id, bc;

	logf("(0) SD_CS = 0x%lX\n", SD_CS);

	PM_SMPS = PM_PASSWORD | 0x1;
	A2W_SMPS_LDO1 = A2W_PASSWORD | 0x40000;
	A2W_SMPS_LDO0 = A2W_PASSWORD | 0x0;

	A2W_XOSC_CTRL |= A2W_PASSWORD | A2W_XOSC_CTRL_DDREN_SET;

	/*
	 * STEP 1:
	 * configure the low-frequency PLL and enable SDC and perform
	 * the calibration sequence.
	 */

	switch_to_cprman_clock(CM_SRC_OSC, 1);

	CALL_INIT_CLKMAN;

	reset_phy();

	/* magic values */
	SD_SA = 0x006E3395;
	SD_SB = 0x0F9;
	SD_SC = 0x6000431;
	SD_SD = 0x10000011;
	SD_SE = 0x10106000;
	SD_PT1 = 0x0AF002;
	SD_PT2 = 0x8C;
	SD_MRT = 0x3;
	SD_CS = 0x200042;

	/* wait for SDRAM controller */
	logf("waiting for SDUP (%lX) ...\n", SD_CS);
	for (;;) if (SD_CS & SD_CS_SDUP_SET) break;
	logf("SDRAM controller has arrived! (%lX)\n", SD_CS);

	/* RL = 6 / WL = 3 */
	write_mr(LPDDR2_MR_DEVICE_FEATURE_2, 4, false);
	calibrate_pvt_early();

	/* identify installed memory */
	vendor_id = read_mr(LPDDR2_MR_MANUFACTURER_ID);
	if (!MR_REQUEST_SUCCESS(vendor_id)) {
		panic("vendor id memory register read timed out");
	}
	vendor_id = MR_GET_RDATA(vendor_id);

	bc = read_mr(LPDDR2_MR_METRICS);
	if (!MR_REQUEST_SUCCESS(bc)) {
		panic("basic configuration memory register read timed out");
	}
	bc = MR_GET_RDATA(bc);

	g_RAMSize = lpddr2_size(bc);

	logf("SDRAM Type: %s %s LPDDR2 (BC=0x%lX)\n",
	     lpddr2_manufacturer_name(vendor_id),
	     size_to_string[g_RAMSize],
	     bc);

	if (g_RAMSize == RAM_SIZE_UNKNOWN)
		panic("unknown ram size (MR8 response was 0x%lX)", bc);

	/*
	 * STEP 2:
	 * after calibration, enable high-freq SDRAM PLL. because we're
	 * running from cache, we can freely mess with SDRAM clock without
	 * any issues, removing the need to copy the SDRAM late init stuff
	 * to bootrom ram. if later code that's running from SDRAM wants to
	 * mess with SDRAM clock it would need to do that.
	 */

	if (g_RAMSize == RAM_SIZE_1GB) {
		g_InitSdramParameters.colbits = 3;
		g_InitSdramParameters.rowbits = 3;
		g_InitSdramParameters.banklow = 3;
	} else if (g_RAMSize == RAM_SIZE_512MB) {
		g_InitSdramParameters.colbits = 2;
	}

	reset_with_timing(&g_InitSdramParameters);
	init_late();
	selftest();
}
