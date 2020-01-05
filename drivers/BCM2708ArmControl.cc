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

extern uint8_t _binary_arm_chainloader_build_arm_chainloader_bin_start;
extern uint8_t _binary_arm_chainloader_build_arm_chainloader_bin_end;

#define ARM_MEMORY_BASE 0xC0000000
#define ARM_BKPT_OPCODE 0xE1200070

void bzero(void *addr, size_t len) {
  uint8_t *x = (uint8_t*)addr;
  for (size_t i = 0; i < len; i++) {
    x[i] = 0;
  };
}

static const uint8_t g_BrespTab[] = {
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

  void setupOtherClocks() {
    // `compatible = "brcm,bcm2835-cprman";` in DT expects PLLA and PLLD to be functioning
    // enable them here
    A2W_XOSC_CTRL |= A2W_PASSWORD | A2W_XOSC_CTRL_PLLAEN_SET;
    A2W_XOSC_CTRL |= A2W_PASSWORD | A2W_XOSC_CTRL_PLLDEN_SET;
  }

	void patchFirmwareData() {
		volatile firmware_arm_data_t* firmware_data = reinterpret_cast<firmware_arm_data_t*>(ARM_MEMORY_BASE + 32);

		firmware_data->sdram_size = g_RAMSize;
		firmware_data->vpu_cpuid = g_CPUID;
	}

	void loadInitialCode() {
		uint32_t* mem = (uint32_t*)(ARM_MEMORY_BASE);

		uint8_t* start = &_binary_arm_chainloader_build_arm_chainloader_bin_start;
		uint8_t* end = &_binary_arm_chainloader_build_arm_chainloader_bin_end;
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

  // maps a 16mb chunk of ram
  // the bus address has a resolution of 2mb
  // the arm addr has a resolution of 16mb
  // the entire mapping is 16mb long
  // comments say there are 32 slots in the list (512mb mapped) an another 32 spare (1gig mapped)
  void mapBusToArm(uint32_t busAddr, uint32_t armAddr) {
    volatile uint32_t* tte = reinterpret_cast<volatile uint32_t*>(&ARM_TRANSLATE);

    uint32_t index = armAddr >> 24; // div by 16mb
    uint32_t pte = busAddr >> 21; // div by 2mb
    IODriverLog("mapBusToArm index:%x, pte:%x", index, pte);

    tte[index] = pte;
  }

  void printregs() {
    uint32_t c0 = ARM_CONTROL0, c1 = ARM_CONTROL1;
    IODriverLog("C0: %x C1: %x", c0, c1);
  }
void bzero2(void *addr, size_t len) {
  uint8_t *x = (uint8_t*)addr;
  for (size_t i = 0; i < len; i++) {
    if ((i % 0x1000000) == 0) IODriverLog("%x", i);
    x[i] = 0;
  };
}
	virtual void start() override {
		IODriverLog("starting ...");

		pmDomain = PowerManagementDomain::getDeviceForDomain(kCprPowerDomainARM);
		assert(pmDomain);

                //IODriverLog("about to zero out, stack near %x", pmDomain);
                //bzero2((void*)ARM_MEMORY_BASE, 1024*1024*512);
                //IODriverLog("zeroed");
		loadInitialCode();

		IODriverLog("original memstart: 0x%X", *((volatile uint32_t*)ARM_MEMORY_BASE));

                // map 62 chunks of 16mb of ram, total of 992mb
		for (uint32_t i = 0; i < 62; i++) {
			uint32_t offset = i * 0x1000000;
			mapBusToArm(ARM_MEMORY_BASE + offset, 0x0 + offset);
                        IODriverLog("mapped VC 0x%X to ARM 0x%X", ARM_MEMORY_BASE + offset, 0x0 + offset);
		}

		//IODriverLog("mapped VC 0x%X to ARM 0x%X", ARM_MEMORY_BASE, 0);

		mapBusToArm(VC4_PERIPH_BASE, ARM_PERIPH_BASE);

		IODriverLog("mapped peripherals VC 0x%X to ARM 0x%X", VC4_PERIPH_BASE, ARM_PERIPH_BASE);

		/* see if the ARM block is responding */
		IODriverLog("ARM ID: 0x%X C0: 0x%X", ARM_ID, ARM_CONTROL0);

		/*
		 * enable peripheral access, map arm secure bits to axi secure bits 1:1 and
		 * set the mem size for who knows what reason.
		 */
                printregs();
		ARM_CONTROL0 |= 0x008 | ARM_C0_APROTPASS | ARM_C0_SIZ1G | ARM_C0_FULLPERI; // | ARM_C0_AARCH64;
                printregs();
		ARM_CONTROL1 |= ARM_C1_PERSON;
                printregs();

		ARM_IRQ_ENBL3 |= ARM_IE_MAIL;

		IODriverLog("using C0: 0x%X", ARM_CONTROL0);
                printregs();

		setupClock();
                //setupOtherClocks();
                printregs();
		pmDomain->start();
                printregs();

		/*
		 * ARM is now powered on but stalling on a bus transaction, start the
		 * async bridge and let ARM start fetching instructions.
		 */
		bridgeStart(true);
                printregs();

		IODevice::start();
                printregs();
	}

	virtual void init() override {
		setTag('ARMC');
	}
};

IODriverCreateSingletonInstance(BCM2708ArmControl);

