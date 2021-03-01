#pragma once

#ifdef __cplusplus
extern "C" {
#endif
  extern uint32_t xtal_freq;

  uint32_t compute_pll_freq(uint32_t ctrl, uint32_t frac);
  uint32_t clk_get_input_freq(volatile uint32_t *ctlreg);
  uint32_t clk_get_freq(volatile uint32_t *divreg, volatile uint32_t *ctlreg);
  uint32_t get_vpu_per_freq(void);
  uint32_t get_uart_base_freq(void);
  uint32_t plla(void);
  uint32_t pllb(void);
  uint32_t pllc(void);
  uint32_t plld(void);
  uint32_t pllh(void);
  uint32_t pllc_core0(void);
#ifdef __cplusplus
}
#endif
