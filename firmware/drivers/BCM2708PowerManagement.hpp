/*
 * VideoCore4_Drivers
 * Copyright (c) 2017 Kristina Brooks
 *
 * BCM2708 power management driver.
 */

#pragma once
#include <drivers/IODevice.hpp>

enum cpr_power_result_t {
	kCprSuccess = 0,
	kCprPowOkTimeout,
	kCprMrDoneTimeout,
	kCprOscCountTimeout
};

enum cpr_power_domain_t {
	kCprPowerDomainImage = 0,
	kCprPowerDomainARM,
	kCprPowerDomainUSB,
	kCprPowerDomainVPU1,

	kCprPowerDomain_MAX
};

struct PowerManagementDomain : IODevice {
	static PowerManagementDomain* getDeviceForDomain(cpr_power_domain_t domain);
	virtual void setReset() = 0;
};