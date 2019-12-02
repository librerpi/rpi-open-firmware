/*
 * VideoCore4_Drivers
 * Copyright (c) 2017 Kristina Brooks
 *
 * ARM control driver.
 */

#include <drivers/IODevice.hpp>
#include <drivers/BCM2708PowerManagement.hpp>
#include <drivers/BCM2708ClockDomains.hpp>

#define FLAG_BUSY (1 << 31)

extern uint32_t g_CPUID;
extern uint32_t g_RAMSize;

extern uint8_t L_arm_code_start;
extern uint8_t L_arm_code_end;

#define ARM_MEMORY_BASE 0xC0000000
#define ARM_BKPT_OPCODE 0xE1200070

static uint8_t g_BrespTab[] = {
	0x10, 0x14, 0x10, 0x14, 0x10, 0x14, 0x10, 0x14, 0x10, 0x1C, 0x18, 0x1C, 0x18, 0x0,
	0x10, 0x14, 0x10, 0x1C, 0x10, 0x14, 0x10, 0x14, 0x10, 0x14, 0x10, 0x14, 0x10, 0x0,
	0x10, 0x14, 0x10, 0x1C, 0x18, 0x1C, 0x10, 0x14, 0x18, 0x1C, 0x10, 0x14, 0x10, 0x0,
	0x10, 0x14, 0x18, 0x14, 0x10, 0x14, 0x10, 0x14, 0x10, 0x14, 0x10, 0x14, 0x10, 0x0,
	0x10, 0x14, 0x18, 0x14, 0x18, 0x14, 0x10, 0x14, 0x10, 0x14, 0x10, 0x14, 0x18, 0x0
};

struct BCM2708ArmControl : IODevice {
	IODriverConstructor(BCM2708ArmControl);

	PowerManagementDomain* pmDomain;

	static inline void bridgeWriteBresp(uint8_t bits) {
		ARM_CONTROL0 = (ARM_CONTROL0 & ~(ARM_C0_BRESP1|ARM_C0_BRESP2)) | bits;
		udelay(30);
	}

	void bridgeCycleBresp() {
		IODriverLog("cycling through bresp bits ...");
		for (int i = 0; i < sizeof(g_BrespTab)/sizeof(g_BrespTab[0]); i++) {
			bridgeWriteBresp(g_BrespTab[i]);
		}
	}

	void bridgeStart(bool cycleBrespBits) {
		IODriverLog("setting up async bridge ...");

		if (cycleBrespBits) {
			pmDomain->setReset();
			bridgeCycleBresp();
			pmDomain->setReset();

			udelay(300);
		}

		IODriverLog("starting async bridge now!");
		ARM_CONTROL1 &= ~ARM_C1_REQSTOP;
		udelay(300);

		if (!cycleBrespBits)
			pmDomain->setReset();

		IODriverLog("bridge init done, PM_PROC is now: 0x%X!", PM_PROC);
	}

	void setupClock() {
		IODriverLog("initializing PLLB ...");

		/* oscillator->pllb */
		A2W_XOSC_CTRL |= A2W_PASSWORD | A2W_XOSC_CTRL_PLLBEN_SET;

		A2W_PLLB_FRAC = A2W_PASSWORD | 0xeaaa8;
		A2W_PLLB_CTRL = A2W_PASSWORD | 48 | 0x1000;

		CM_PLLB = CM_PASSWORD | CM_PLLB_DIGRST_SET | CM_PLLB_ANARST_SET;
		CM_PLLB = CM_PASSWORD | CM_PLLB_DIGRST_SET | CM_PLLB_ANARST_SET | CM_PLLB_HOLDARM_SET;

		A2W_PLLB_ANA3 = A2W_PASSWORD | 0x100;
		A2W_PLLB_ANA2 = A2W_PASSWORD | 0x0;
		A2W_PLLB_ANA1 = A2W_PASSWORD | 0x140000;
		A2W_PLLB_ANA0 = A2W_PASSWORD | 0x0;

		BCM2708VCO* pllb = static_cast<BCM2708VCO*>(IODevice::findByTag('PLLB'));
		assert(pllb);
		pllb->setDigValues();

		A2W_PLLB_ARM = A2W_PASSWORD | 2;

		CM_PLLB = CM_PASSWORD | CM_PLLB_DIGRST_SET | CM_PLLB_ANARST_SET | CM_PLLB_HOLDARM_SET | CM_PLLB_LOADARM_SET;
		CM_PLLB = CM_PASSWORD | CM_PLLB_DIGRST_SET | CM_PLLB_ANARST_SET | CM_PLLB_HOLDARM_SET;
		CM_PLLB = CM_PASSWORD;

		CM_ARMCTL = CM_PASSWORD | 4 | CM_ARMCTL_ENAB_SET;

		IODriverLog("KAIP  = 0x%X", A2W_PLLB_ANA_KAIP); /* 0x228 */
		IODriverLog("MULTI = 0x%X", A2W_PLLB_ANA_MULTI); /* 0x613277 */

		IODriverLog("ARM clock succesfully initialized!");
	}

	void patchFirmwareData() {
		volatile firmware_arm_data_t* firmware_data = reinterpret_cast<firmware_arm_data_t*>(ARM_MEMORY_BASE + 32);

		firmware_data->sdram_size = g_RAMSize;
		firmware_data->vpu_cpuid = g_CPUID;
	}

	void loadInitialCode() {
		uint32_t* mem = (uint32_t*)(ARM_MEMORY_BASE);

		uint8_t* start = &L_arm_code_start;
		uint8_t* end = &L_arm_code_end;
		uint32_t size = (uint32_t)(end - start);

		bcopy(start, mem, size);

		IODriverLog("copied %d bytes to 0x%X!", size, ARM_MEMORY_BASE);

		/* verify */
		for (int i = 0; i < size; i++) {
			uint8_t* mem8 = (uint8_t*)(mem);
			if (start[i] != mem8[i])
				panic("copy failed at 0x%lX expected 0x%lX, got 0x%lX", (uint32_t)&mem8[i],
				      *((uint32_t*)&mem8[i]),
				      *((uint32_t*)&start[i]));
		}

		patchFirmwareData();
	}

	void mapBusToArm(uint32_t busAddr, uint32_t armAddr) {
		volatile uint32_t* tte = reinterpret_cast<volatile uint32_t*>(&ARM_TRANSLATE);

		uint32_t index = armAddr >> 24;
		uint32_t pte = busAddr >> 21;

		tte[index] = pte;
	}

	virtual void start() override {
		IODriverLog("starting ...");

		pmDomain = PowerManagementDomain::getDeviceForDomain(kCprPowerDomainARM);
		assert(pmDomain);

		loadInitialCode();

		IODriverLog("original memstart: 0x%X", *((volatile uint32_t*)ARM_MEMORY_BASE));

		for (uint32_t i = 0; i < 62; i++) {
			uint32_t offset = i * 0x1000000;
			mapBusToArm(ARM_MEMORY_BASE + offset, 0x0 + offset);
		}

		IODriverLog("mapped VC 0x%X to ARM 0x%X", ARM_MEMORY_BASE, 0);

		mapBusToArm(VC4_PERIPH_BASE, ARM_PERIPH_BASE);

		IODriverLog("mapped peripherals VC 0x%X to ARM 0x%X", VC4_PERIPH_BASE, ARM_PERIPH_BASE);

		/* see if the ARM block is responding */
		IODriverLog("ARM ID: 0x%X C0: 0x%X", ARM_ID, ARM_CONTROL0);

		/*
		 * enable peripheral access, map arm secure bits to axi secure bits 1:1 and
		 * set the mem size for who knows what reason.
		 */
		ARM_CONTROL0 |= 0x008 | ARM_C0_APROTPASS | ARM_C0_SIZ1G | ARM_C0_FULLPERI;
		ARM_CONTROL1 |= ARM_C1_PERSON;

		ARM_IRQ_ENBL3 |= ARM_IE_MAIL;

		IODriverLog("using C0: 0x%X", ARM_CONTROL0);

		setupClock();
		pmDomain->start();

		/*
		 * ARM is now powered on but stalling on a bus transaction, start the
		 * async bridge and let ARM start fetching instructions.
		 */
		bridgeStart(true);

		IODevice::start();
	}

	virtual void init() override {
		setTag('ARMC');
	}
};

IODriverCreateSingletonInstance(BCM2708ArmControl);

