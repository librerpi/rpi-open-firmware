#pragma once

struct uart_device {
  volatile uint32_t *data_reg;
  volatile uint32_t *lsr_reg;
};
