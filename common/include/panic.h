#pragma once

#ifdef __cplusplus
extern "C" {
#endif

extern void panic(const char* fmt,  ...)
	__attribute__((noreturn))
	__attribute__ ((format (printf, 1, 2)));

#define panic_plain(ex, ...) \
	(panic)(ex, ## __VA_ARGS__)
#define __STRINGIFY(x) #x
#define LINE_NUMBER(x) __STRINGIFY(x)
#define PANIC_LOCATION __FILE__ ":" LINE_NUMBER(__LINE__)
#define panic(ex, ...) \
	(panic)(# ex "@" PANIC_LOCATION, ## __VA_ARGS__)

#define assert(x) if (!(x)) { panic("assertion '%s' failed", #x); }

#ifdef __cplusplus
}
#endif