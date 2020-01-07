#include <stdint.h>
#include <stdio.h>
#include "aux.h"
#include <hardware.h>

uint32_t clk_get_freq(volatile uint32_t *divreg, volatile uint32_t *ctlreg);
void uart_init(uint32_t freq, uint32_t baud);

void main_entry() {
  uint32_t freq = clk_get_freq(&CM_VPUDIV, &CM_VPUCTL);
  uint32_t t = GP_FSEL1;
  t &= ~(7<<12) & ~(7<<15); // clear FSEL for gpio 14&15
  t |= (2<<12) |  (2<<15); // set mode 2 on 14&15
  GP_FSEL1 = t;

  uart_init(freq, 115200);
  setup_aux_uart();

  puts("hello from c");
}

#undef AUX_MU_LCR_REG
#undef AUX_MU_MCR_REG
#undef AUX_MU_CNTL_REG
#undef AUX_MU_IER_REG

#define AUXXENG *((volatile uint32_t*)(0x7E215004))
#define AUX_MU_IIR_REG *((volatile uint32_t*)(0x7E215044))
#define AUX_MU_IER_REG *((volatile uint32_t*)(0x7E215048))
#define AUX_MU_LCR_REG *((volatile uint32_t*)(0x7E21504c))
#define AUX_MU_MCR_REG *((volatile uint32_t*)(0x7E215050))
#define AUX_MU_CNTL_REG *((volatile uint32_t*)(0x7E215060))
#define AUX_MU_BAUD *((volatile uint32_t*)(0x7E215068))

void uart_init(uint32_t freq, uint32_t baud) {
  AUXXENG = 1;
  AUX_MU_IIR_REG = 0;
  AUX_MU_CNTL_REG = 0;
  AUX_MU_LCR_REG = 3;

  AUX_MU_MCR_REG = 0;
  AUX_MU_IIR_REG = 0;
  AUX_MU_IER_REG = 0xc6;

  uint32_t aux_divisor = (freq / (baud * 8)) - 1;
  AUX_MU_BAUD = aux_divisor;
  AUX_MU_LCR_REG = 3;
  AUX_MU_CNTL_REG = 3; // enable tx and rx
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
