#pragma once

#include <lib/stdarg.h>

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

#ifndef __cplusplus
	typedef int bool;
	#define true 1
	#define false 0
#endif

#define ALWAYS_INLINE __attribute__((always_inline)) inline

#define _OPEN_SOURCE

