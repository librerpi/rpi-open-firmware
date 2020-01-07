#include <stdio.h>
#include <stdint.h>

#include "uart.h"

void uart_write(struct uart_device *uart, const char *str, int length);
void uart_putchar(struct uart_device *uart, char c);

#define AUX_MU_IO_REG  *((volatile uint32_t*)(0x7E215040))
#define AUX_MU_LSR_REG *((volatile uint32_t*)(0x7E215054))

struct uart_device aux_uart = { &AUX_MU_IO_REG, &AUX_MU_LSR_REG };

void setup_aux_uart() {
  stdout = funopen(&aux_uart, 0, uart_write, 0, 0);
  setvbuf(stdout, 0, _IONBF, 0);
}

void uart_write(struct uart_device *uart, const char *str, int length) {
  for (int i=0; i<length; i++) {
    uart_putchar(uart, str[i]);
  }
}

void uart_putchar(struct uart_device *uart, char c) {
  if (c == '\n') uart_putchar(uart, '\r');
  while ((*(uart->lsr_reg) & 0x20) == 0);
  *(uart->data_reg) = c;
}
