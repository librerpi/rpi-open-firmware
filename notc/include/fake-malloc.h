#ifndef HAS_DYNAMIC_ALLOCATIONS
static inline void* malloc(size_t size) { panic("malloc: dynamic allocations not supported on this configuration"); }
static inline void free(void* obj)      { panic("free: dynamic allocations not supported on this configuration"); }
#endif
