#pragma once

struct uart_device {
  volatile uint32_t *data_reg;
  volatile uint32_t *lsr_reg;
};

extern struct uart_device aux_uart;
extern FILE *aux_fd;

void setup_aux_uart(uint32_t baud);
void aux_flush();
