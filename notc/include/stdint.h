#ifndef __STDINT_H
#define __STDINT_H
#include <stddef.h>
#if __SIZEOF_INT__ == 4
typedef unsigned int uint32_t;
#else
#warning "unknown uint32_t"
#endif
#if __SIZEOF_SHORT__ == 2
typedef unsigned short uint16_t;
#else
#warning "unknown uint16_t"
#endif
typedef unsigned char uint8_t;
#if __SIZEOF_LONG_LONG__ == 8
typedef unsigned long long uint64_t;
#else
#warning "unknown uint64_t"
#endif
#if __SIZEOF_LONG__ == __SIZEOF_POINTER__
typedef unsigned long uintptr_t;
#else
#warning "unknown uintptr_t"
#endif
#endif /* ndef __STDINT_H */
