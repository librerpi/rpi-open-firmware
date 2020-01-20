#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

#include <panic.h>

#ifdef __arm__
#define HAS_DYNAMIC_ALLOCATIONS
#endif

#ifdef HAS_DYNAMIC_ALLOCATIONS
#include <tlsf/tlsf.h>
#endif

#ifdef __VIDEOCORE4__
#include <vc4_types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define RegToRef(x) reinterpret_cast<volatile uint32_t*>(&x)

extern void udelay(uint32_t time);
extern void __cxx_init();

#ifdef __cplusplus
}
#endif
