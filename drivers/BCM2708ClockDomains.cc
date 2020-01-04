/*
 * VideoCore4_Drivers
 * Copyright (c) 2017 Kristina Brooks
 *
 * PLL VCOs and their channels.
 */

#include <drivers/BCM2708ClockDomains.hpp>

/*
PLL/Channel tree:
|-XOSC
 |-PLLA
   |-CORE
   |-PER
   |-CCP2
   |-DSI0
 |-PLLB
   |-ARM
     |-ARM Clock (CM_ARM*)
   |-SP0 (???)
   |-SP1 (???)
   |-SP2 (???)
 |-PLLC
   |-CORE0
   |-CORE1
   |-PER
   |-CORE2 (???)
 |-PLLD
   |-DSI0
   |-DSI1
   |-CORE
   |-PER
 |-PLLH
   |-AUX
   |-PIX
   |-RCAL
   |-STS
 */

/***********************************************************************
 *
 * Common VCO stuff.
 *
 ***********************************************************************/

void BCM2708VCO::setDigValues() {
	IODriverLog("setting DIG values for this VCO ...");

	uint32_t dig0 = dig[0],
		dig1 = dig[1],
		dig2 = dig[2],
		dig3 = dig[3];

	/* 0xAA covers all possible HOLD bits in CM_PLLx regs */
        // for PLLA: HOLDDSI0, HOLDCCP2, HOLDCORE, HOLDPER amd DIGRST
        // for PLLB: HOLDARM and DIGRST
        // for PLLC: HOLDCORE0, HOLDCORE1, HOLDCORE2, HOLDPER, DIGRST
        // for PLLD: HOLDDSI0, HOLDDSI1, HOLDCORE, HOLDPER, DIGRST
	*cmPllCtrl = CM_PASSWORD | 0xAA;

	dig[3] = A2W_PASSWORD | (dig3);
	dig[2] = A2W_PASSWORD | (dig2 & 0xFFEFFBFE);
	dig[1] = A2W_PASSWORD | (dig1 & ~(1 << 14));
	dig[0] = A2W_PASSWORD | (dig0);

	*ctrl = A2W_PASSWORD | 0x20000 | *ctrl; // set PRSTN bit

	dig3 |= 0x42;

	dig[3] = A2W_PASSWORD | (dig3);
	dig[2] = A2W_PASSWORD | (dig2);
	dig[1] = A2W_PASSWORD | (dig1);
	dig[0] = A2W_PASSWORD | (dig0);
}

void BCM2708VCO::dumpDigValues() {
	IODriverLog("DIG0: 0x%X, DIG1: 0x%X, DIG2: 0x%X, DIG3: 0x%X",
		dig[0],
		dig[1],
		dig[2],
		dig[3]);
}

void BCM2708VCO::configure(uint32_t ndiv, uint32_t frac, uint32_t pdiv, bool halve) {
  // based on bcm2835_pll_set_rate from the linux kernel
  uint32_t ana[4];
  bool do_early_ana_setup;

  for (int i=3; i>=0; i--) ana[i] = this->ana[i];
  bool was_using_prediv = ana[1] & prediv_mask;

  ana[0] &= ~ana0_mask;
  ana[0] |= ana0_set;

  ana[1] &= ~ana1_mask;
  ana[1] |= ana1_set;

  ana[3] &= ~ana3_mask;
  ana[3] |= ana3_set;

  if (was_using_prediv && !halve) {
    ana[1] &= ~prediv_mask;
    do_early_ana_setup = true;
  } else if (!was_using_prediv && halve) {
    ana[1] |= prediv_mask;
    do_early_ana_setup = false;
  } else do_early_ana_setup = true;

  // unmask the input clock to the PLL
  A2W_XOSC_CTRL |= A2W_PASSWORD | reference_enable;

  if (do_early_ana_setup) write_ana(ana);

  *(this->frac) = frac;

  uint32_t a2w_ctl = *ctrl;
  a2w_ctl &= ~(A2W_PLLC_CTRL_NDIV_SET | A2W_PLLC_CTRL_PDIV_SET); // clear all bits used by ndiv and pdiv
  a2w_ctl |= ndiv;
  a2w_ctl |= pdiv << A2W_PLLC_CTRL_PDIV_LSB;
  *ctrl = A2W_PASSWORD | a2w_ctl;

  if (!do_early_ana_setup) write_ana(ana);
}

void BCM2708VCO::write_ana(uint32_t ana[4]) {
  for (int i = 3; i >= 0; i--) this->ana[i] = A2W_PASSWORD | ana[i];
}

BCM2835PLLDivider::BCM2835PLLDivider(const char *name, volatile uint32_t *a2w, volatile uint32_t *cm, uint8_t channel) : a2w(a2w), cm(cm), channel(channel) {
  setName(name);
}

void BCM2835PLLDivider::configure(uint32_t divisor) {
  uint32_t a2w = *this->a2w;
  a2w &= ~A2W_PLLC_PER_DIV_SET;
  a2w |= divisor & A2W_PLLC_PER_DIV_SET;
  *this->a2w = A2W_PASSWORD | a2w;

  uint32_t cm = *this->cm;
  *this->cm = CM_PASSWORD | cm | (1 << (channel * 2));
  *this->cm = CM_PASSWORD | (cm & ~(1 << (channel * 2)));

  // A2W_PLLC_PER_CHENB_SET is either the enable or disable flag
  // name says enable, but linux source calls it disable and treats it as disable
}

/***********************************************************************
 *
 * PLL VCOs for specific PLLs.
 *
 ***********************************************************************/

struct BCM2708PLLB : BCM2708VCO {
	IODriverConstructor(BCM2708PLLB);

	/*
	 * Working: DIG0: 0xAAA030, DIG1: 0x403A, DIG2: 0x402401, DIG3: 0x2
	 */

	virtual void init() override {
		ctrl = RegToRef(A2W_PLLB_CTRL);
		ana = RegToRef(A2W_PLLB_ANA0);
		dig = RegToRef(A2W_PLLB_DIG0);
		cmPllCtrl = RegToRef(CM_PLLB);

		IODriverLog("PLLB VCO registered");

		setTag('PLLB');
	}
};
IODriverCreateSingletonInstance(BCM2708PLLB);

/***********************************************************************
 * 
 * ARM Clock.
 *
 ***********************************************************************/

struct BCM2708ClockDomain : IODevice {
	volatile uint32_t* cmDiv;
	volatile uint32_t* cmCtrl;
};

struct BCM2708ArmClockDomain : BCM2708ClockDomain {

};

BCM2835PLLDivider pllc_per("PLLC_PER", &A2W_PLLC_PER, &CM_PLLC, 3);
