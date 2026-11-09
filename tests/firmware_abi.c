#include <stddef.h>

#include "common/hardware.h"

_Static_assert(offsetof(firmware_arm_data_t, sdram_size) == 0,
               "SDRAM field moved");
_Static_assert(offsetof(firmware_arm_data_t, reserved) == 16,
               "reserved fields moved");
_Static_assert(sizeof(firmware_arm_data_t) == 60,
               "firmware data storage must preserve the extended ABI size");
_Static_assert(FW_RESERVED_UART_CLOCK == 0,
               "UART clock field moved");
_Static_assert(FW_RESERVED_FB_MAGIC == 1,
               "framebuffer magic field moved");
_Static_assert(FW_RESERVED_FB_FORMAT == 10,
               "framebuffer format field moved");
_Static_assert(FRAMEBUFFER_HANDOFF_MAGIC == 0x3142464cU,
               "framebuffer handoff magic changed");

int main(void) {
  return 0;
}
#include "arm_chainloader/cpu_id.h"

_Static_assert((0x410fb767U & ARM_MIDR_PART_MASK) == ARM_MIDR_PART_ARM1176,
               "BCM2835 must use the single-core path");
_Static_assert((0x410fc075U & ARM_MIDR_PART_MASK) != ARM_MIDR_PART_ARM1176,
               "Cortex-A7 must use the SMP path");
_Static_assert((0x410fd034U & ARM_MIDR_PART_MASK) != ARM_MIDR_PART_ARM1176,
               "Cortex-A53 must use the SMP path");
