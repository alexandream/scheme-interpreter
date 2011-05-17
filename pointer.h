#ifndef __POINTER_H__
#define __POINTER_H__

#include "value.h"

static inline
void * unwrap_pointer(value_t value) {
	// XXX: Assumption #001
	return (void*) (value & 0xFFFFFFFFFFFFFFFC);
}

#endif
