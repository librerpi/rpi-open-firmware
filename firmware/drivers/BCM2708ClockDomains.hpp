/*
 * VideoCore4_Drivers
 * Copyright (c) 2017 Kristina Brooks
 *
 * PLL VCOs and their channels.
 */

#pragma once
#include <drivers/IODevice.hpp>

struct BCM2708VCO : IODevice {
	volatile uint32_t* dig;
	volatile uint32_t* ana;
	volatile uint32_t* ctrl;
	volatile uint32_t* cmPllCtrl;
        volatile uint32_t *frac;

	void setDigValues();
	void dumpDigValues();
        void configure(uint32_t ndiv, uint32_t frac, uint32_t pdiv, bool halve);
        void write_ana(uint32_t ana[4]);
private:
  uint32_t prediv_mask;
  uint32_t ana0_mask, ana0_set, ana1_mask, ana1_set, ana3_mask, ana3_set;
  uint32_t reference_enable; // for example A2W_XOSC_CTRL_PLLDEN_SET
};

struct BCM2835PLLDivider : IODevice {
  BCM2835PLLDivider(const char *name, volatile uint32_t *a2w, volatile uint32_t *cm, uint8_t channel);
  void configure(uint32_t divisor);
private:
  volatile uint32_t *a2w; // for example A2W_PLLC_PER
  volatile uint32_t *cm; // for example CM_PLLC
  uint8_t channel; // 0/1/2/3, which pair of load/hold bits to use in CM
};

extern BCM2835PLLDivider pllc_per;
