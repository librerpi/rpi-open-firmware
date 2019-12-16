/*
 * VideoCore4_Drivers
 * Copyright (c) 2017 Kristina Brooks
 *
 * BCM2708 power management driver.
 */

#include <drivers/BCM2708PowerManagement.hpp>

#define PM_UNK_CFG_CLR 0xFFFCFFFF

PowerManagementDomain* g_BCM2708PowerDomains[kCprPowerDomain_MAX];

PowerManagementDomain* PowerManagementDomain::getDeviceForDomain(cpr_power_domain_t domain) {
	if (domain > kCprPowerDomain_MAX-1) {
		return nullptr;
	}
	PowerManagementDomain* pm = g_BCM2708PowerDomains[domain];
}

/***********************************************************************
 *
 * Superclass for all power domains.
 *
 ***********************************************************************/

struct BCM2708PowerDomain : PowerManagementDomain {
	volatile uint32_t* pmReg;
	uint32_t rstnMask;
	cpr_power_domain_t powerDomain;

	inline void pmvOr(uint32_t& pmv, uint32_t value) {
		pmv |= value;
		*pmReg = pmv;
	}

	inline void pmvAnd(uint32_t& pmv, uint32_t value) {
		pmv &= value;
		*pmReg = pmv;
	}

	bool waitForPmBit(uint32_t bit) {
		for (int i = 0; i < 20; i++) {
			if (*pmReg & bit) {
				return true;
			}
			waitOscTicks();
		}
		return false;
	}

	cpr_power_result_t waitForPOWOK(uint32_t& pmv) {
		IODriverLog("waiting for POWOK ...");

		for (int i = 1; i < 5; i++) {
			if (!waitForPmBit(PM_GNRIC_POWOK_SET)) {
				/* only go up to 3 */
				if (i == 4) {
					IODriverLog("timed out waiting for powok!");
					return kCprPowOkTimeout;
				}

				pmv = (pmv & PM_UNK_CFG_CLR) | (i << PM_GNRIC_CFG_LSB);
				IODriverLog("timed out, trying a different cfg value: 0x%X", pmv);
				*pmReg = pmv;
			}
		}

		IODriverLog("got POWOK with CFG=0x%X", *pmReg & PM_GNRIC_CFG_SET);
		return kCprSuccess;
	}

	cpr_power_result_t beginPowerUpSequence(uint32_t& pmv) {
		IODriverLog("starting power up sequence ...");
		pmvOr(pmv, PM_GNRIC_POWUP_SET);
	}

	cpr_power_result_t completePowerUpSequence(uint32_t& pmv, bool setPowup = true) {
		pmvOr(pmv, PM_GNRIC_ISPOW_SET);
		pmvOr(pmv, PM_GNRIC_MEMREP_SET);

		IODriverLog("waiting for MRDONE ...");
		if (!waitForPmBit(PM_GNRIC_MRDONE_SET)) {
			IODriverLog("timed out waiting for MRDONE: 0x%X", *pmReg);
			return kCprMrDoneTimeout;
		}

		pmvOr(pmv, PM_GNRIC_ISFUNC_SET);

		return kCprSuccess;
	}

	virtual cpr_power_result_t powerOn() {
		uint32_t pmv;
		cpr_power_result_t res;

		IODriverLog("powering on (rstnMask=0x%X) ...", rstnMask);

		pmv = ((*pmReg & rstnMask) & PM_UNK_CFG_CLR) | PM_PASSWORD;
		*pmReg = pmv;
		udelay(15);

		beginPowerUpSequence(pmv);

		res = waitForPOWOK(pmv);
		if (res != kCprSuccess)
			return res;

		res = completePowerUpSequence(pmv);
		if (res != kCprSuccess)
			return res;

		IODriverLog("domain powered on succesfully");

		return kCprSuccess;
	}

	virtual void setReset() override {
		IODriverLog("setting RSTN bits to 0x%X ...", (~rstnMask));
		*pmReg |= (~rstnMask) | PM_PASSWORD;
		udelay(300);
	}

	void waitOscTicks(uint32_t ticks = 2) {
		CM_OSCCOUNT = CM_PASSWORD | ticks;
		while(CM_OSCCOUNT != 0);
	}

	template <typename T>
	void setPmReg(T regAddr) {
		pmReg = reinterpret_cast<volatile uint32_t*>(regAddr);
	}

	void registerPowerDomain(cpr_power_domain_t domain) {
		g_BCM2708PowerDomains[domain] = this;
	}
};

/***********************************************************************
 *
 * Image power domain.
 * Contains: PERII (USB), H264, ISP
 *
 ***********************************************************************/

struct BCM2708PowerDomainImage : BCM2708PowerDomain {
	IODriverConstructor(BCM2708PowerDomainImage);

	virtual void init() override {
		setName("BCM2708PowerDomainImage");
		setPmReg(0x7E100108);
		registerPowerDomain(kCprPowerDomainImage);
		rstnMask = ~(PM_IMAGE_ISPRSTN_SET | PM_IMAGE_H264RSTN_SET | PM_IMAGE_PERIRSTN_SET);
	}

	virtual cpr_power_result_t powerOn() {
		uint32_t pmv;
		cpr_power_result_t res;

		IODriverLog("powering on, current PM_IMAGE state: 0x%X", PM_IMAGE);

		pmv = *pmReg | 0x10000 | PM_PASSWORD;
		*pmReg = pmv;

		/*
		 * first sequence has to be done with this CFG.
		 */
		beginPowerUpSequence(pmv);

		res = waitForPOWOK(pmv);
		if (res != kCprSuccess)
			return res;

		IODriverLog("stopping power up sequence ...");
		pmvAnd(pmv, ~PM_GNRIC_POWUP_SET);
		pmvOr(pmv, 0x30000); /* CFG */

		/*
		 * now do this all over again with the new CFG.
		 */
		beginPowerUpSequence(pmv);

		res = waitForPOWOK(pmv);
		if (res != kCprSuccess)
			return res;

		res = completePowerUpSequence(pmv);
		if (res != kCprSuccess)
			return res;

		BCM2708PowerDomain::stop();

		return kCprSuccess;
	}

	void resetPeripheralsUngated() {
		CM_PERIICTL = CM_PASSWORD | CM_PERIICTL_GATE_SET;

#ifdef __VIDEOCORE4__
		IODriverLog("calling v16mov gated ...");

		/*
		 * don't ask me.
		 */
		__asm volatile (
			"v16mov -, 0 REP32\n"
			"v16mov -, 0\n"
		);
#endif

		IODriverLog("ungating and resetting ...");

		CM_PERIICTL = CM_PASSWORD;

		waitOscTicks();

		*pmReg |= CM_PASSWORD | PM_IMAGE_PERIRSTN_SET;

		CM_PERIICTL = CM_PASSWORD | CM_PERIICTL_GATE_SET;

		IODriverLog("done");
	}

	virtual void start() override {
#if 1
		cpr_power_result_t res = powerOn();

		if (res != kCprSuccess)
			panic("%s failed to start the image power domain", driverName);

		resetPeripheralsUngated();
#else
		IODriverLog("image_domain_peripherals_init returned: 0x%X", image_domain_peripherals_init());
#endif

		IODriverLog("CM_PERIICTL = 0x%X", CM_PERIICTL);
		IODriverLog("CM_PERIIDIV = 0x%X", CM_PERIIDIV);
		IODriverLog("PM_IMAGE    = 0x%X", PM_IMAGE);

		IODriverLog("started");

		BCM2708PowerDomain::start();
	}
};

IODriverCreateSingletonInstance(BCM2708PowerDomainImage);

/***********************************************************************
 *
 * USB power domain.
 *
 ***********************************************************************/

struct BCM2708PowerDomainUSB : BCM2708PowerDomain {
	IODriverConstructor(BCM2708PowerDomainUSB);

	virtual void init() override {
		setName("BCM2708PowerDomainUSB");
		setPmReg(&PM_USB);
		registerPowerDomain(kCprPowerDomainUSB);
	}

	virtual void stop() override {
		IODriverLog("stopping ...");

		PM_USB = PM_PASSWORD;
		udelay(200000);

		IODriverLog("stopped");

		BCM2708PowerDomain::stop();
	}

	virtual void start() override {
		IODriverLog("starting ...");

		PM_USB |= PM_PASSWORD | PM_USB_CTRLEN_SET;
		udelay(600);

		IODriverLog("started");

		BCM2708PowerDomain::start();
	}
};

IODriverCreateSingletonInstance(BCM2708PowerDomainUSB);

/***********************************************************************
 *
 * ARM power domain.
 *
 ***********************************************************************/

struct BCM2708PowerDomainARM : BCM2708PowerDomain {
	IODriverConstructor(BCM2708PowerDomainARM);

	virtual void init() override {
		setName("BCM2708PowerDomainARM");
		setPmReg(&PM_PROC);
		rstnMask = PM_PROC_ARMRSTN_CLR;
		registerPowerDomain(kCprPowerDomainARM);
	}

	virtual void stop() override {
		panic("this power domain cannot be stopped");
	}

	virtual void start() override {
		IODriverLog("starting ...");

		cpr_power_result_t res = powerOn();

		if (res != kCprSuccess)
			panic("%s failed to start the ARM power domain", driverName);

		IODriverLog("started");

		BCM2708PowerDomain::start();
	}
};

IODriverCreateSingletonInstance(BCM2708PowerDomainARM);

