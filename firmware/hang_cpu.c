#include <hang_cpu.h>
#include "cpu.h"

void __attribute__((noreturn)) hang_cpu() {
  _hang_cpu();
}
