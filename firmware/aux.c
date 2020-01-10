#include <stdio.h>
#include <stdint.h>

#include "uart.h"
#include "pll_read.h"
#include "aux.h"

#define AUXXENG *((volatile uint32_t*)(0x7E215004))
#define AUX_MU_IO_REG  *((volatile uint32_t*)(0x7E215040))
#define AUX_MU_IIR_REG *((volatile uint32_t*)(0x7E215044))
#define AUX_MU_IER_REG *((volatile uint32_t*)(0x7E215048))
#define AUX_MU_LCR_REG *((volatile uint32_t*)(0x7E21504c))
#define AUX_MU_MCR_REG *((volatile uint32_t*)(0x7E215050))
#define AUX_MU_LSR_REG *((volatile uint32_t*)(0x7E215054))
#define AUX_MU_CNTL_REG *((volatile uint32_t*)(0x7E215060))
#define AUX_MU_BAUD *((volatile uint32_t*)(0x7E215068))

struct uart_device aux_uart = { &AUX_MU_IO_REG, &AUX_MU_LSR_REG };

void aux_uart_init(uint32_t baud) {
  uint32_t freq = get_vpu_per_freq();
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

void setup_aux_uart(uint32_t baud) {
  aux_uart_init(baud);
  stdout = funopen(&aux_uart, 0, uart_write, 0, 0);
  setvbuf(stdout, 0, _IONBF, 0);
}
