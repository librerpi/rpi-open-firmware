#include <hardware.h>
#include "pll_read.h"

uint32_t get_vpu_per_freq() {
  return clk_get_freq(&CM_VPUDIV, &CM_VPUCTL);
}

uint32_t pllc() {
  uint32_t ana1 = A2W_PLLC_ANA1;
  uint32_t ctrl = A2W_PLLC_CTRL;
  uint32_t frac = A2W_PLLC_FRAC & A2W_PLLC_FRAC_MASK;
  uint32_t ndiv = A2W_PLLC_CTRL & A2W_PLLC_CTRL_NDIV_SET;
  uint32_t pdiv = (A2W_PLLC_CTRL & A2W_PLLC_CTRL_PDIV_SET) >> A2W_PLLC_CTRL_PDIV_LSB;
  uint64_t mult1 = (ndiv << 20) | frac;
  mult1 *= pdiv;
  // TODO, the optional /2 phase
  uint32_t freq = (54000000 * mult1) >> 20;
  return freq;
}

uint32_t pllc_core0() {
  uint32_t ctrl = A2W_PLLC_CORE0;
  uint32_t div = ctrl & A2W_PLLC_CORE0_DIV_SET;
  uint32_t pllc_freq = pllc();
  return pllc_freq / div;
}

uint32_t clk_get_freq(volatile uint32_t *divreg, volatile uint32_t *ctlreg) {
  uint32_t div = *divreg;
  if (div == 0) return 0;
  uint32_t ctl = *ctlreg;
  switch (ctl & 0xf) {
  case 0: // GND clock source
    return 0;
  case 1: // crystal oscilator
    return 54000000; // rpi4 xtal is 54mhz
  case 2: // test debug 0
  case 3: // test debug 1
    return 0;
  case 4: // plla
    return 0;
  case 5: // pllc_core0
    return pllc_core0();
  case 6: // plld_per
    return 0;
  case 7: // pllh_aux
    return 0;
  case 8: // pllc_core1?
    return 0;
  case 9: // pllc_core2?
    return 0;
  default:
    return 0;
  }
}
