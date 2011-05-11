#ifndef __POINTER_H__
#define __POINTER_H__

#include "value.h"

static inline
value_t wrap_pointer(void* ptr) {
	// XXX: Assumption #001
	return ((uint64_t) ptr) | 0x01;
}

static inline
void * unwrap_pointer(value_t value) {
	// XXX: Assumption #001
	return (void*) (value & 0xFFFFFFFFFFFFFFF0);
}

#endif
