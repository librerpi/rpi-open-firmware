#pragma once

#include <stdint.h>

struct uart_device {
  volatile uint32_t *data_reg;
  volatile uint32_t *lsr_reg;
};

int uart_write(struct uart_device *uart, const char *str, int length);
void uart_putchar(struct uart_device *uart, char c);
