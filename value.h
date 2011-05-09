#ifndef __VALUE_H__
#define __VALUE_H__

#include <stdint.h>

typedef uint64_t value_t;

static inline
uint8_t get_tag(value_t value) {
	return (uint8_t) (value & 0x1F);
}
#endif

