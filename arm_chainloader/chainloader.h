#pragma once

#include <runtime.h>
#include <stdint.h>
#include <stddef.h>
#include <memory_map.h>
#include "arm-defs.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void __attribute__((noreturn)) _hang_cpu() {
  __asm__ __volatile__ ("wfi\n");

  /* in case the above fails */
  for (;;) {
    __asm__ __volatile__ ("nop\nnop\nnop\nnop\nnop\nnop");
    __asm__ __volatile__ ("wfi\n");
  }
}

#define STATIC_INIT_PRIORITY(x) __attribute__((init_priority(x)))

#define STATIC_CPRMAN_DRIVER STATIC_INIT_PRIORITY(101)
#define STATIC_DRIVER STATIC_INIT_PRIORITY(200)
#define STATIC_FILESYSTEM STATIC_INIT_PRIORITY(300)
#define STATIC_APP STATIC_INIT_PRIORITY(600)

#define mfence() __sync_synchronize()

#define NBBY 8

#define	__BIT(__n)	\
    (((uintmax_t)(__n) >= NBBY * sizeof(uintmax_t)) ? 0 : \
    ((uintmax_t)1 << (uintmax_t)((__n) & (NBBY * sizeof(uintmax_t) - 1))))

static inline uint32_t arm_get_cpsr() {
  uint32_t r;
  __asm__ volatile("mrs %0, cpsr\n" : "=r" (r) :: "memory");
  return r;
}

static inline uint32_t arm_get_midr() {
  uint32_t r;
  __asm__("mrc p15, 0, %0, c0, c0, 0" : "=r"(r));
  return r;
}

#ifdef __cplusplus
}
#endif

