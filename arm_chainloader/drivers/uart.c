#include <runtime.h>
#include <hardware.h>

#define UART_DR     (UART_BASE+0x00)
#define UART_FR     (UART_BASE+0x18)

void uart_putc(unsigned int ch)
{
     while(mmio_read32(UART_FR) & 0x20);
     mmio_write32(UART_DR, ch);
}
