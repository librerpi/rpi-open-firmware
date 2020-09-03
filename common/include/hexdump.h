#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void hexdump_ram(volatile void *realaddr, uint32_t reportaddr, uint32_t count);
void safe_putchar(unsigned char c);

#ifdef __cplusplus
}
#endif
