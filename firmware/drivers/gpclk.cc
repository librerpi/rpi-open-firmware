#include "drivers/gpclk.hh"

GPCLK GPCLK0 = GPCLK(&CM_GP0CTL, &CM_GP0DIV, "GPCLK0");
GPCLK GPCLK1 = GPCLK(&CM_GP1CTL, &CM_GP1DIV, "GPCLK1");
GPCLK GPCLK2 = GPCLK(&CM_GP2CTL, &CM_GP2DIV, "GPCLK2");

GPCLK::GPCLK(volatile uint32_t *ctrl_addr, volatile uint32_t *div_addr, const char *name) {
  ctrl = ctrl_addr;
  divider = div_addr;
  setName(name);
}
