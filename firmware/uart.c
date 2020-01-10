#include "uart.h"

int uart_write(struct uart_device *uart, const char *str, int length) {
  for (int i=0; i<length; i++) {
    uart_putchar(uart, str[i]);
  }
  return length;
}

void uart_putchar(struct uart_device *uart, char c) {
  if (c == '\n') uart_putchar(uart, '\r');
  while ((*(uart->lsr_reg) & 0x20) == 0);
  *(uart->data_reg) = c;
}
