#ifndef __STRING_H
#define __STRING_H
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void memcpy(void* dest, const void* src, size_t n); /* builtin */
void memset(void* s, int c, size_t n);
int strlen(const char* s);

#ifdef __cplusplus
}
#endif

#endif /* __STRING_H */
