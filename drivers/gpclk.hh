#include <drivers/IODevice.hpp>

enum MashControl {
  kMashControlNone = 0,
  kMashControl1 = 1,
  kMashControl2 = 2,
  kMashControl3 = 3
};

struct GPCLK : IODevice {
  GPCLK(volatile uint32_t *ctrl_addr, volatile uint32_t *div_addr, const char *name);
private:
  volatile uint32_t *ctrl;
  volatile uint32_t *divider;
};

extern GPCLK GPCLK0;
extern GPCLK GPCLK1;
extern GPCLK GPCLK2;
